#!/bin/bash

cd "$(dirname "$0")/.."

memory_create_command="./build/app/memory --create"
memory_remove_command="./build/app/memory --remove"

actuator_command="./build/app/actuator"
dashboard_command="./build/app/dashboard --control"

if [ "$(whoami)" != "root" ] ; then
  echo "This script requires superuser privileges" 1>&2
  exit 1
fi

memory_create () {
  $memory_create_command
  return $?
}

memory_remove () {
  $memory_remove_command
  return $?
}

actuator_start () {
  $actuator_command &
  actuator_process=$!
}

actuator_stop () {
  kill -int $actuator_process
}

actuator_running () {
  if [ "$(ps -p $actuator_process -o command=)" == "$actuator_command" ] ; then
    return 0
  else
    return 1
  fi
}

dashboard_start () {
  $dashboard_command
}

if ! memory_create ; then
  exit 1
fi

actuator_start
dashboard_start

if actuator_running ; then
  actuator_stop
fi

while actuator_running ; do
  :
done

if ! memory_remove ; then
  exit 1
fi

exit 0
