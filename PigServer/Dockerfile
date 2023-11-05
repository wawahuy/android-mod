FROM node:18.16.1-alpine3.18 as build
WORKDIR /usr/src/app
COPY --chown=node:node ./ ./
RUN yarn build

###################
# PRODUCTION
###################
FROM node:18.16.1-alpine3.18 As production
WORKDIR /usr/src/app
COPY --chown=node:node --from=build /usr/src/app/node_modules ./node_modules
COPY --chown=node:node --from=build /usr/src/app/dist ./dist

# fix exec bin
RUN apk update
USER node
ENV NODE_ENV production

CMD ["node", "dist/main.js"]