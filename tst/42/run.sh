#!/bin/bash
source			./conf.env

docker build	--build-arg OS=$image					\
				--build-arg OS_RELEASE=$image_release	\
				-t $build_name:$build_version . || exit $?

docker run		--rm														\
				--name $build_name											\
				-it															\
				-p $port:$port										        \
                -v $project_root:/src                                       \
				$build_name:$build_version || exit $?
