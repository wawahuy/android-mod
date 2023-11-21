

#!/bin/bash

currentDir=$(dirname "$0")

source "${currentDir}/env_file.txt"

SSH_USERNAME=${1:-"${SSH_USERNAME}"}
SSH_HOST=${2:-"${SSH_HOST}"}
echo "SSH: ${SSH_HOST}, USERNAME: ${SSH_USERNAME}"

# common
source "${currentDir}/common/common.sh"

# push ssh key
source "${currentDir}/common/ssh.sh"
pushSshKey "$SSH_USERNAME" "$SSH_HOST"


# Function to start port forwarding
start_forwarding() {
  echo "Starting port forwarding..."
  echo "+ 27017 -> 127.0.0.1:27018"
  ssh -fN -L 27018:127.0.0.1:27018 $SSH_USERNAME@$SSH_HOST &
}

# Function to stop port forwarding
stop_forwarding() {
  echo "Stopping port forwarding..."
  pkill -f "ssh -fN -L 27018:127.0.0.1:27018 $SSH_USERNAME@$SSH_HOST"
  isExit=1
}

# Start port forwarding
start_forwarding

trap stop_forwarding SIGINT

# Trap SIGINT signal to stop port forwarding when the script is terminated
while [ -z "$isExit" ]; do
  sleep 1
done