#!/bin/bash
currentDir=$(dirname "$0")

source "${currentDir}/env_file.txt"

SSH_USERNAME=${1:-"${SSH_USERNAME}"}
SSH_HOST=${2:-"${SSH_HOST}"}
FILE_DOCKER_SETUP="${currentDir}/common/setup-docker.sh"

echo "SSH: ${SSH_HOST}, USERNAME: ${SSH_USERNAME}"

# common
source "${currentDir}/common/common.sh"

# push ssh key
source "${currentDir}/common/ssh.sh"
pushSshKey "$SSH_USERNAME" "$SSH_HOST"

# install docker
cmdGetDockerVersion="docker version --format '{{.Server.Version}}'"
dockerVersion=$(runCommandRemote "$cmdGetDockerVersion")
if [ -z "$dockerVersion" ]; then
  echo "Docker installing..."
  fileDockerRemote="~/setup-docker.sh"
  scp "$FILE_DOCKER_SETUP" "${SSH_USERNAME}@${SSH_HOST}:~/"
  runCommandRemote "chmod +x $fileDockerRemote"
  runCommandRemote "$fileDockerRemote"
  dockerVersion=$(runCommandRemote "$cmdGetDockerVersion")
fi

if [ -z "${dockerVersion}" ]; then
  echo "Docker not installed"
  exit;
fi

echo "Docker installed! version: $dockerVersion"

#####################
# build & push
runCommandRemote "mkdir -p ~/xhpigmod-v2"

# push libso
# runCommandRemote "rm -rf ~/xhpigmod-v2/libso"
# scp -r "${currentDir}/../libso/" "${SSH_USERNAME}@${SSH_HOST}:~/xhpigmod-v2"

imageXhPigModServer="xhpigmod-v2-server:latest"
fileXhPigModServer="${currentDir}/../data-docker/xhpigmod-v2-server.tar"
fileXhPigModServerRemote="~/xhpigmod-v2/xhpigmod-v2-server.tar"
fileXhPigModServerContinue=1

mkdir -p "${currentDir}/../data-docker"

if ssh -q -o ConnectTimeout=10 "${SSH_USERNAME}@${SSH_HOST}" "[ -f ${fileXhPigModServerRemote} ]"; then
    echo "File ${fileXhPigModServerRemote} exists on the remote server."
    read -p "You can overwrite ${fileXhPigModServerRemote}? (y/n): " answer
    if [[ "$answer" == "y" || "$answer" == "Y" ]]; then
      fileXhPigModServerContinue=1
      echo "${fileXhPigModServerRemote} overwrite it!"
    else
      fileXhPigModServerContinue=0
    fi
else
    echo "File ${fileXhPigModServerRemote} does not exist on the remote server."
fi

# build backend
if [ "$fileXhPigModServerContinue" -eq 1 ]; then
  echo "Building server..."
  docker build -f "${currentDir}/../Dockerfile" -t "${imageXhPigModServer}" "${currentDir}/.."

  echo "Save server image to file..."
  if [ -f "${fileXhPigModServer}" ]; then
    echo "Remove server image old..."
    rm -f "${fileXhPigModServer}"
  fi
  docker save -o "${fileXhPigModServer}" "${imageXhPigModServer}"

  echo "Remove server image..."
  docker rmi "${imageXhPigModServer}"
fi

# stop & remove image
cmdRemoveImage=" \
cd ~/xhpigmod-v2;
docker compose --env-file ~/xhpigmod-v2/.env.prod -f ./docker-compose.xhpigmod.yaml down; \
"
echo "Container Stoping..."
runCommandRemote "$cmdRemoveImage"

# push env
scp "${currentDir}/../env/.env.prod" "${SSH_USERNAME}@${SSH_HOST}:~/xhpigmod-v2"

# push docker-compose file to remote
scp "${currentDir}/../docker-compose.xhpigmod.yaml" "${SSH_USERNAME}@${SSH_HOST}:~/xhpigmod-v2"

# push backend to remote
if [ "$fileXhPigModServerContinue" -eq 1 ]; then
  echo "Push server image..."
  scp "$fileXhPigModServer" "${SSH_USERNAME}@${SSH_HOST}:~/xhpigmod-v2"
  runCommandRemote "docker rmi ${imageXhPigModServer}; docker load -i ${fileXhPigModServerRemote};"
fi

# deploy web
cmdDeploy=" \
cd ~/xhpigmod-v2;
docker compose --env-file ~/xhpigmod-v2/.env.prod -f ./docker-compose.xhpigmod.yaml up -d --remove-orphans --build --force-recreate; \
"
runCommandRemote "$cmdDeploy"