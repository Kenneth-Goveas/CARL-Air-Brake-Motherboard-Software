#!/bin/bash

cd "$(dirname "$0")/.."

interrupted=false
trap interrupted=true SIGINT

memory_create_command="./build/app/memory --create"
memory_remove_command="./build/app/memory --remove"

actuator_command="./build/app/actuator"
instrument_command="./build/app/instrument"
controller_command="./build/app/controller"
estimator_command="./build/app/estimator"
detector_command="./build/app/detector"
recorder_command="./build/app/recorder --control --estimate --sensing"

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

instrument_start () {
  $instrument_command &
  instrument_process=$!
}

instrument_stop () {
  kill -int $instrument_process
}

instrument_running () {
  if [ "$(ps -p $instrument_process -o command=)" == "$instrument_command" ] ; then
    return 0
  else
    return 1
  fi
}

controller_start () {
  $controller_command &
  controller_process=$!
}

controller_stop () {
  kill -int $controller_process
}

controller_running () {
  if [ "$(ps -p $controller_process -o command=)" == "$controller_command" ] ; then
    return 0
  else
    return 1
  fi
}

estimator_start () {
  $estimator_command &
  estimator_process=$!
}

estimator_stop () {
  kill -int $estimator_process
}

estimator_running () {
  if [ "$(ps -p $estimator_process -o command=)" == "$estimator_command" ] ; then
    return 0
  else
    return 1
  fi
}

detector_start () {
  $detector_command &
  detector_process=$!
}

detector_stop () {
  kill -int $detector_process
}

detector_running () {
  if [ "$(ps -p $detector_process -o command=)" == "$detector_command" ] ; then
    return 0
  else
    return 1
  fi
}

recorder_start () {
  $recorder_command &
  recorder_process=$!
}

recorder_stop () {
  kill -int $recorder_process
}

recorder_running () {
  if [ "$(ps -p $recorder_process -o command=)" == "$recorder_command" ] ; then
    return 0
  else
    return 1
  fi
}

if ! memory_create ; then
  shutdown now
fi

actuator_start
instrument_start
controller_start
estimator_start
detector_start
recorder_start

while ! $interrupted ; do
  if ! actuator_running ; then
    break
  fi
  if ! instrument_running ; then
    break
  fi
  if ! controller_running ; then
    break
  fi
  if ! estimator_running ; then
    break
  fi
  if ! detector_running ; then
    break
  fi
  if ! recorder_running ; then
    break
  fi
done

if actuator_running ; then
  actuator_stop
fi

if instrument_runing ; then
  instrument_stop
fi

if controller_runing ; then
  controller_stop
fi

if estimator_runing ; then
  estimator_stop
fi

if detector_runing ; then
  detector_stop
fi

if recorder_runing ; then
  recorder_stop
fi

while actuator_running ; do
  :
done

while instrument_running ; do
  :
done

while controller_running ; do
  :
done

while estimator_running ; do
  :
done

while detector_running ; do
  :
done

while recorder_running ; do
  :
done

if ! memory_remove ; then
  shutdown now
fi

shutdown now
