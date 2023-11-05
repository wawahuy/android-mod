#!/bin/bash

function pushSshKey() {
  local hasSshKey=0
  local username=$1
  local host=$2
  if ssh -q -o ConnectTimeout=10 "${username}@${host}" '[ -f ~/.ssh/authorized_keys ]'; then
      echo "File ~/.ssh/authorized_keys exists on the remote server."
      hasSshKey=1
  else
      echo "File ~/.ssh/authorized_keys does not exist on the remote server."
      hasSshKey=0
  fi

  if [ "$hasSshKey" -eq 0 ]; then
    echo "Push ssh public key..."
    cat ~/.ssh/id_rsa_gitpod.pub | ssh ${username}@${host} "mkdir -p ~/.ssh && touch ~/.ssh/authorized_keys && chmod -R go= ~/.ssh && cat >> ~/.ssh/authorized_keys"

    echo "Config ssh..."
    remoteCommand="sed -i 's/^PasswordAuthentication yes/PasswordAuthentication no/' /etc/ssh/sshd_config"
    if ssh "${username}@${host}" "${remoteCommand}"; then
      echo "Ssh config done!"
      ssh "${username}@${host}" "systemctl restart ssh"
    else
      echo "Ssh config failed!"
      exit
    fi
  fi
}
