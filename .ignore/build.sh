#!/bin/sh
DEBUG_BUILD=1
RELEASE_BUILD=1

# for arg in $*
# do 
# 	if [ "$arg" = "debug" ]; then
# 		export DEBUG_BUILD
# 	elif [ "$arg" = "rel" ]; then
# 		export RELEASE_BUILD
# 	fi
# done




for arg in $*; do :; done


if "$arg" = "debug"; then 
	export DEBUG_BUILD
elif "$arg" = "rel"; then
	export RELEASE_BUILD
fi


make $*
