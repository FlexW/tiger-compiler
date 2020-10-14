#!/bin/bash -x

mkdir -p m4
exec autoreconf --install "$@"
