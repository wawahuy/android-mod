#!/bin/bash

id=$(id -u)
uname=$(id -nu)
envFile="env/.env.dev"

echo "uname:$uname & id:$id"

function updateEnv() {
  local varName=$1
  local varValue=$2
  if grep -q "^${varName}=" "${envFile}"; then
    sed -i "s/^${varName}=.*/${varName}=${varValue}/" "$envFile"
  else
    echo "${varName}=${varValue}" >> "$envFile"
  fi
}

updateEnv "XH_PIGMOD_H_NAME" "$uname"
updateEnv "XH_PIGMOD_H_ID" "$id"