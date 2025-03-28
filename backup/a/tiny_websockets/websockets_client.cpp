#include <tiny_websockets/internals/ws_common.hpp>
#include <tiny_websockets/network/tcp_client.hpp>
#include <tiny_websockets/message.hpp>
#include <tiny_websockets/client.hpp>
#include <tiny_websockets/internals/wscrypto/crypto.hpp>

namespace websockets {
    WebsocketsClient::WebsocketsClient() : WebsocketsClient(std::make_shared<WSDefaultTcpClient>()) {
        // Empty
    }

    WebsocketsClient::WebsocketsClient(std::shared_ptr<network::TcpClient> client) : 
        _client(client),
        _endpoint(client), 
        _connectionOpen(client->available()),
        _messagesCallback([](WebsocketsClient&, WebsocketsMessage){}),
        _eventsCallback([](WebsocketsClient&, WebsocketsEvent, WSInterfaceString){}),
        _sendMode(SendMode_Normal) {
        // Empty
        _id = client->getSocket();
    }

    WebsocketsClient::WebsocketsClient(const WebsocketsClient& other) : 
        _client(other._client),
        _endpoint(other._endpoint),
        _connectionOpen(other._client->available()),
        _messagesCallback(other._messagesCallback),
        _eventsCallback(other._eventsCallback),
        _sendMode(other._sendMode) {
        _id = other._id;
        // delete other's client
        const_cast<WebsocketsClient&>(other)._client = nullptr;
        const_cast<WebsocketsClient&>(other)._connectionOpen = false;
    }
    
    WebsocketsClient::WebsocketsClient(const WebsocketsClient&& other) : 
        _client(other._client),
        _endpoint(other._endpoint),
        _connectionOpen(other._client->available()),
        _messagesCallback(other._messagesCallback),
        _eventsCallback(other._eventsCallback),
        _sendMode(other._sendMode) {

        _id = other._id;
        // delete other's client
        const_cast<WebsocketsClient&>(other)._client = nullptr;
        const_cast<WebsocketsClient&>(other)._connectionOpen = false;
    }
    
    WebsocketsClient& WebsocketsClient::operator=(const WebsocketsClient& other) {
        _id = other._id;

        // call endpoint's copy operator
        _endpoint = other._endpoint;

        // get callbacks and data from other
        this->_client = other._client;
        this->_messagesCallback = other._messagesCallback;
        this->_eventsCallback = other._eventsCallback;
        this->_connectionOpen = other._connectionOpen;
        this->_sendMode = other._sendMode;
    
        // delete other's client
        const_cast<WebsocketsClient&>(other)._client = nullptr;
        const_cast<WebsocketsClient&>(other)._connectionOpen = false;
        return *this;
    }

    WebsocketsClient& WebsocketsClient::operator=(const WebsocketsClient&& other) {
        _id = other._id;

        // call endpoint's copy operator
        _endpoint = other._endpoint;

        // get callbacks and data from other
        this->_client = other._client;
        this->_messagesCallback = other._messagesCallback;
        this->_eventsCallback = other._eventsCallback;
        this->_connectionOpen = other._connectionOpen;
        this->_sendMode = other._sendMode;
    
        // delete other's client
        const_cast<WebsocketsClient&>(other)._client = nullptr;
        const_cast<WebsocketsClient&>(other)._connectionOpen = false;
        return *this;
    }

    struct HandshakeRequestResult {
        WSString requestStr;
        WSString expectedAcceptKey;
    };
    HandshakeRequestResult generateHandshake(const WSString& host, const WSString& uri, 
                                             const std::vector<std::pair<WSString, WSString>>& customHeaders) {
        
        WSString key = crypto::base64Encode(crypto::randomBytes(16));

        WSString handshake = "GET " + uri + " HTTP/1.1\r\n";
        handshake += "Host: " + host + "\r\n";
        handshake += "Upgrade: websocket\r\n";
        handshake += "Connection: Upgrade\r\n";
        handshake += "Sec-WebSocket-Key: " + key + "\r\n";
        handshake += "Sec-WebSocket-Version: 13\r\n";
        handshake += "User-Agent: TinyWebsockets Client\r\n";
        handshake += "Origin: https://github.com/gilmaimon/TinyWebsockets\r\n";

        for (const auto& header: customHeaders) {
            handshake += header.first + ": " + header.second + "\r\n";
        }

        handshake += "\r\n";

        HandshakeRequestResult result;
        result.requestStr = handshake;
#ifndef _WS_CONFIG_SKIP_HANDSHAKE_ACCEPT_VALIDATION
        result.expectedAcceptKey = crypto::websocketsHandshakeEncodeKey(key);
#endif
        return std::move(result);
    }

    bool isWhitespace(char ch) {
        return ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n';
    }

    struct HandshakeResponseResult {
        bool isSuccess;
        WSString serverAccept;
    };

    bool isCaseInsensetiveEqual(const WSString lhs, const WSString rhs) {
      if (lhs.size() != rhs.size()) return false;
      
      for (size_t i = 0; i < lhs.size(); i++) {
        char leftLowerCaseChar = lhs[i] >= 'A' && lhs[i] <= 'Z' ? lhs[i] - 'A' + 'a' : lhs[i];
        char righerLowerCaseChar = rhs[i] >= 'A' && rhs[i] <= 'Z' ? rhs[i] - 'A' + 'a' : rhs[i];
        if (leftLowerCaseChar != righerLowerCaseChar) return false;
      }

      return true;
    }

    HandshakeResponseResult parseHandshakeResponse(std::vector<WSString> responseHeaders) {
        bool didUpgradeToWebsockets = false, isConnectionUpgraded = false;
        WSString serverAccept = "";
        for(WSString header : responseHeaders) {            
            auto colonIndex = header.find_first_of(':');

            WSString key = header.substr(0, colonIndex);
            WSString value = header.substr(colonIndex + 2); // +2 (ignore space and ':')

            if(isCaseInsensetiveEqual(key, "upgrade")) {
                didUpgradeToWebsockets = isCaseInsensetiveEqual(value, "Websocket");
            } else if(isCaseInsensetiveEqual(key, "connection")) {
                isConnectionUpgraded = isCaseInsensetiveEqual(value, "upgrade");
            } else if(isCaseInsensetiveEqual(key, "Sec-WebSocket-Accept")) {
                serverAccept = value;
            }
        }

        HandshakeResponseResult result;
        result.isSuccess = serverAccept != "" && didUpgradeToWebsockets && isConnectionUpgraded;
        result.serverAccept = serverAccept;
        return result;
    }

    bool doestStartsWith(WSString str, WSString prefix) {
        if(str.size() < prefix.size()) return false;
        for(size_t i = 0; i < prefix.size(); i++) {
            if(str[i] != prefix[i]) return false;
        }

        return true;
    }

    void WebsocketsClient::upgradeToSecuredConnection() {
    #ifndef _WS_CONFIG_NO_SSL
        this->_client = std::make_shared<WSDefaultSecuredTcpClient>();
        this->_endpoint.setInternalSocket(this->_client);
    #endif //_WS_CONFIG_NO_SSL
    }

    void WebsocketsClient::addHeader(const WSInterfaceString key, const WSInterfaceString value) {
        _customHeaders.push_back({internals::fromInterfaceString(key), internals::fromInterfaceString(value)}); 
    }

    bool WebsocketsClient::connect(WSInterfaceString _url) {
        WSString url = internals::fromInterfaceString(_url);
        WSString protocol = "";
        int defaultPort = 0;

        if(doestStartsWith(url, "http://")) {
            defaultPort = 80;
            protocol = "http";
            url = url.substr(7); //strlen("http://") == 7
        } else if(doestStartsWith(url, "ws://")) {
            defaultPort = 80;
            protocol = "ws";
            url = url.substr(5); //strlen("ws://") == 5
        }

    #ifndef _WS_CONFIG_NO_SSL
        else if(doestStartsWith(url, "wss://")) {
            defaultPort = 443;
            protocol = "wss";
            url = url.substr(6); //strlen("wss://") == 6

            upgradeToSecuredConnection();
        } else if(doestStartsWith(url, "https://")) {
            defaultPort = 443;
            protocol = "https";
            url = url.substr(8); //strlen("https://") == 8
            
            upgradeToSecuredConnection();
        } 
    #endif

        else {
            return false;
            // Not supported
        }

        auto uriBeg = url.find_first_of('/');
        std::string host = url, uri = "/";

        if(static_cast<int>(uriBeg) != -1) {
            uri = url.substr(uriBeg);
            host = url.substr(0, uriBeg);
        }

        auto portIdx = host.find_first_of(':');
        int port = defaultPort;
        if(static_cast<int>(portIdx) != -1) {
            auto onlyHost = host.substr(0, portIdx);
            ++portIdx;
            port = 0;
            while(portIdx < host.size() && host[portIdx] >= '0' && host[portIdx] <= '9') {
                port = port * 10 + (host[portIdx] - '0');
                ++portIdx;
            }

            host = onlyHost;
        }
        
        return this->connect(
            internals::fromInternalString(host), 
            port, 
            internals::fromInternalString(uri)
        );
    }

    bool WebsocketsClient::connect(WSInterfaceString host, int port, WSInterfaceString path) {
        this->_connectionOpen = this->_client->connect(internals::fromInterfaceString(host), port);
        if (!this->_connectionOpen) return false;

        auto handshake = generateHandshake(internals::fromInterfaceString(host), internals::fromInterfaceString(path), _customHeaders);
        this->_client->send(handshake.requestStr);

        auto head = this->_client->readLine();
        if(!doestStartsWith(head, "HTTP/1.1 101")) {
            close(CloseReason_ProtocolError);
            return false;
        }

        std::vector<WSString> serverResponseHeaders;
        WSString line = "";
        while (true) {
            line = this->_client->readLine();
            if (line == "\r\n") break;
            // remove /r/n from line end
            line = line.substr(0, line.size() - 2);
            
            serverResponseHeaders.push_back(line);
        }
        auto parsedResponse = parseHandshakeResponse(serverResponseHeaders);
        
#ifdef _WS_CONFIG_SKIP_HANDSHAKE_ACCEPT_VALIDATION
        bool serverAcceptMismatch = false;
#else
        bool serverAcceptMismatch = parsedResponse.serverAccept != handshake.expectedAcceptKey;
#endif
        if(parsedResponse.isSuccess == false || serverAcceptMismatch) {
            close(CloseReason_ProtocolError);
            return false;
        }

        this->_eventsCallback(*this, WebsocketsEvent::ConnectionOpened, {});
        return true;
    }

    void WebsocketsClient::onMessage(MessageCallback callback) {
        this->_messagesCallback = callback;
    }

    void WebsocketsClient::onMessage(PartialMessageCallback callback) {
        this->_messagesCallback = [callback](WebsocketsClient&, WebsocketsMessage msg) {
            callback(msg);
        };
    }

    void WebsocketsClient::onEvent(EventCallback callback) {
        this->_eventsCallback = callback;
    }

    void WebsocketsClient::onEvent(PartialEventCallback callback) {
        this->_eventsCallback = [callback](WebsocketsClient&, WebsocketsEvent event, WSInterfaceString data) {
            callback(event, data);
        };
    }

    bool WebsocketsClient::poll() {
        bool messageReceived = false;
        while(available() && _endpoint.poll()) {
            auto msg = _endpoint.recv();
            if(msg.isEmpty()) {
                continue;
            }
            messageReceived = true;
            
            if(msg.isBinary() || msg.isText()) {
                this->_messagesCallback(*this, std::move(msg));
            } else if(msg.isContinuation()) {
                // continuation messages will only be returned when policy is appropriate
                this->_messagesCallback(*this, std::move(msg));
            } else if(msg.isPing()) {
                _handlePing(std::move(msg));
            } else if(msg.isPong()) {
                _handlePong(std::move(msg));
            } else if(msg.isClose()) {
                this->_connectionOpen = false;
                _handleClose(std::move(msg));
            }
        }

        return messageReceived;
    }

    WebsocketsMessage WebsocketsClient::readBlocking() {
        while(available()) {
#ifdef PLATFORM_DOES_NOT_SUPPORT_BLOCKING_READ
            while(available() && _endpoint.poll() == false) continue;
#endif
            auto msg = _endpoint.recv();
            if(!msg.isEmpty()) return msg;
        }
        return {};
    }

    bool WebsocketsClient::send(const WSInterfaceString& data) {
        auto str = internals::fromInterfaceString(data);
        return this->send(str.c_str(), str.size());
    }

    bool WebsocketsClient::send(const WSInterfaceString&& data) {
        auto str = internals::fromInterfaceString(data);
        return this->send(str.c_str(), str.size());
    }
    
    bool WebsocketsClient::send(const char* data) {
        return this->send(data, strlen(data));
    }

    bool WebsocketsClient::send(const char* data, const size_t len) {
        if(available()) {
            // if in normal mode
            if(this->_sendMode == SendMode_Normal) {
                // send a normal message
                return _endpoint.send(
                    data,
                    len,
                    internals::ContentType::Text,
                    true
                );
            }
            // if in streaming mode
            else if(this->_sendMode == SendMode_Streaming) {
                // send a continue frame
                return _endpoint.send(
                    data, 
                    len, 
                    internals::ContentType::Continuation,
                    false
                );
            }
        }
        return false;
    }

    bool WebsocketsClient::sendBinary(WSInterfaceString data) {
        auto str = internals::fromInterfaceString(data);
        return this->sendBinary(str.c_str(), str.size());
    }

    bool WebsocketsClient::sendBinary(const char* data, const size_t len) {
        if(available()) {
            // if in normal mode
            if(this->_sendMode == SendMode_Normal) {
                // send a normal message
                return _endpoint.send(
                    data,
                    len,
                    internals::ContentType::Binary,
                    true
                );
            }
            // if in streaming mode
            else if(this->_sendMode == SendMode_Streaming) {
                // send a continue frame
                return _endpoint.send(
                    data, 
                    len, 
                    internals::ContentType::Continuation,
                    false
                );
            }
        }
        return false;
    }

    bool WebsocketsClient::stream(const WSInterfaceString data) {
        if(available() && this->_sendMode == SendMode_Normal) {
            this->_sendMode = SendMode_Streaming;
            return _endpoint.send(
                internals::fromInterfaceString(data), 
                internals::ContentType::Text, 
                false
            );
        }
        return false;
    }

    
    bool WebsocketsClient::streamBinary(const WSInterfaceString data) {
        if(available() && this->_sendMode == SendMode_Normal) {
            this->_sendMode = SendMode_Streaming;
            return _endpoint.send(
                internals::fromInterfaceString(data), 
                internals::ContentType::Binary, 
                false
            );
        }
        return false;
    }

    bool WebsocketsClient::end(const WSInterfaceString data) {
        if(available() && this->_sendMode == SendMode_Streaming) {
            this->_sendMode = SendMode_Normal;
            return _endpoint.send(
                internals::fromInterfaceString(data), 
                internals::ContentType::Continuation, 
                true
            );
        }
        return false;
    }

    void WebsocketsClient::setFragmentsPolicy(const FragmentsPolicy newPolicy) {
        _endpoint.setFragmentsPolicy(newPolicy);
    }

    bool WebsocketsClient::available(const bool activeTest) {
        if(activeTest)  {
            _endpoint.ping("");
        }

        bool updatedConnectionOpen = this->_connectionOpen && this->_client && this->_client->available();
        
        if(updatedConnectionOpen != this->_connectionOpen) {
            _endpoint.close(CloseReason_AbnormalClosure);
            this->_eventsCallback(*this, WebsocketsEvent::ConnectionClosed, "");
        }

        this->_connectionOpen = updatedConnectionOpen;
        return this->_connectionOpen;
    }

    bool WebsocketsClient::ping(const WSInterfaceString data) {
        if(available()) {
            return _endpoint.ping(internals::fromInterfaceString(data));
        }
        return false;
    }

    bool WebsocketsClient::pong(const WSInterfaceString data) {
        if(available()) {
            return _endpoint.pong(internals::fromInterfaceString(data));
        }
        return false;
    }

    void WebsocketsClient::close(const CloseReason reason) {
        if(available()) {
            this->_connectionOpen = false;
            _endpoint.close(reason);
            _handleClose({});
        }
    }

    CloseReason WebsocketsClient::getCloseReason() const {
        return _endpoint.getCloseReason();
    }

    void WebsocketsClient::_handlePing(const WebsocketsMessage message) {
        this->_eventsCallback(*this, WebsocketsEvent::GotPing, message.data());
    }

    void WebsocketsClient::_handlePong(const WebsocketsMessage message) {
        this->_eventsCallback(*this, WebsocketsEvent::GotPong, message.data());
    }

    void WebsocketsClient::_handleClose(const WebsocketsMessage message) {
        this->_eventsCallback(*this, WebsocketsEvent::ConnectionClosed, message.data());
    }

    WebsocketsClient::~WebsocketsClient() {
        if(available()) {
            this->close(CloseReason_GoingAway);
        }
    }
}
