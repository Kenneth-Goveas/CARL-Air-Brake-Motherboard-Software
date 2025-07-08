#!/bin/bash

cd "$(dirname "$0")/.."

interrupted=false
trap interrupted=true SIGINT

calibrator_command="./build/app/calibrator"

if [ "$(whoami)" != "root" ] ; then
  echo "This script requires superuser privileges" 1>&2
  exit 1
fi

calibrator_start () {
  $calibrator_command &
  calibrator_process=$!
}

calibrator_stop () {
  kill -int $calibrator_process
}

calibrator_running () {
  if [ "$(ps -p $calibrator_process -o command=)" == "$calibrator_command" ] ; then
    return 0
  else
    return 1
  fi
}

calibrator_start

while ! $interrupted ; do
  if ! calibrator_running ; then
    break
  fi
done

if calibrator_running ; then
  calibrator_stop
fi

while calibrator_running ; do
  :
done

exit 0
