#!/bin/bash

cd "$(dirname "$0")/.."

memory_create_command='./build/unit/memory --create --control'
memory_remove_command='./build/unit/memory --remove --control'

actuator_command='./build/unit/actuator'
dashboard_command='./build/unit/dashboard --control'

if [ "$(whoami)" != "root" ] ; then
  echo "This script requires superuser privileges" 1>&2
  exit
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
  exit
fi

actuator_start
dashboard_start

if actuator_running ; then
  actuator_stop
fi

while actuator_running ; do
  sleep 0
done

memory_remove
