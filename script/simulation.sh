#!/bin/bash

cd "$(dirname "$0")/.."

interrupted=false
trap interrupted=true SIGINT

memory_create_command="./build/app/memory --create"
memory_remove_command="./build/app/memory --remove"

simulator_command="./build/app/simulator"
controller_command="./build/app/controller"
estimator_command="./build/app/estimator"
detector_command="./build/app/detector"
streamer_command="./build/app/streamer --control --state --estimate --sensing"
recorder_command="./build/app/recorder --control --state --estimate --sensing"

memory_create () {
  $memory_create_command
  return $?
}

memory_remove () {
  $memory_remove_command
  return $?
}

simulator_start () {
  $simulator_command &
  simulator_process=$!
}

simulator_stop () {
  kill -int $simulator_process
}

simulator_running () {
  if [ "$(ps -p $simulator_process -o command=)" == "$simulator_command" ] ; then
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

streamer_start () {
  $streamer_command &
  streamer_process=$!
}

streamer_stop () {
  kill -int $streamer_process
}

streamer_running () {
  if [ "$(ps -p $streamer_process -o command=)" == "$streamer_command" ] ; then
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
  exit 1
fi

simulator_start
controller_start
estimator_start
detector_start
streamer_start
recorder_start

while ! $interrupted ; do
  if ! simulator_running ; then
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
  if ! streamer_running ; then
    break
  fi
  if ! recorder_running ; then
    break
  fi
done

if simulator_running ; then
  simulator_stop
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

if streamer_running ; then
  streamer_stop
fi

if recorder_running ; then
  recorder_stop
fi

while simulator_running ; do
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

while streamer_running ; do
  :
done

while recorder_running ; do
  :
done

if ! memory_remove ; then
  exit 1
fi

exit 0
