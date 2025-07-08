#!/bin/bash

cd "$(dirname "$0")/.."

interrupted=false
trap interrupted=true SIGINT

memory_create_command="./build/app/memory --create"
memory_remove_command="./build/app/memory --remove"

instrument_command="./build/app/instrument"
estimator_command="./build/app/estimator"
streamer_command="./build/app/streamer --estimate --sensing"

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

if ! memory_create ; then
  exit 1
fi

instrument_start
estimator_start
streamer_start

while ! $interrupted ; do
  if ! instrument_running ; then
    break
  fi
  if ! estimator_running ; then
    break
  fi
  if ! streamer_running ; then
    break
  fi
done

if instrument_running ; then
  instrument_stop
fi

if estimator_running ; then
  estimator_stop
fi

if streamer_running ; then
  streamer_stop
fi

while instrument_running ; do
  :
done

while estimator_running ; do
  :
done

while streamer_running ; do
  :
done

if ! memory_remove ; then
  exit 1
fi

exit 0
