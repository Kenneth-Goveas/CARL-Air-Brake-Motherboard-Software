#!/bin/bash

cd "$(dirname "$0")/.."

interrupted=false
trap interrupted=true SIGINT

memory_create_command='./build/unit/memory --create --control --estimate --sensing'
memory_remove_command='./build/unit/memory --remove --control --estimate --sensing'

actuator_command='./build/unit/actuator'
instrument_command='./build/unit/instrument'
controller_command='./build/unit/controller'
estimator_command='./build/unit/estimator'
detector_command='./build/unit/detector'
recorder_command='./build/unit/recorder --control --estimate --sensing'

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
  exit
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

if instrument_running ; then
  instrument_stop
fi

if controller_running ; then
  controller_stop
fi

if estimator_running ; then
  estimator_stop
fi

if detector_running ; then
  detector_stop
fi

if recorder_running ; then
  recorder_stop
fi

while actuator_running ; do
  sleep 0
done

while instrument_running ; do
  sleep 0
done

while controller_running ; do
  sleep 0
done

while estimator_running ; do
  sleep 0
done

while detector_running ; do
  sleep 0
done

while recorder_running ; do
  sleep 0
done

memory_remove

shutdown now
