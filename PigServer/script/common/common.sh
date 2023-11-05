#!/bin/bash

SSH_PORT=${SSH_PORT:-"22"}

if [ -z $SSH_USERNAME ] || [ -z $SSH_HOST ]; then
  echo "need SSH_USERNAME, SSH_HOST variable";
  exit
fi

function runCommandRemote() {
  local cmd=$1
  ssh "${SSH_USERNAME}@${SSH_HOST}" -p $SSH_PORT "$cmd"
}

function runCommandRemoteI() {
  local cmd=$1
  ssh -i ${SSH_KEY} "${SSH_USERNAME}@${SSH_HOST}" -p $SSH_PORT "$cmd"
}