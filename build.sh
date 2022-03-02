#!/bin/bash

# Arguments
if [ -z "$1" ]
then
    echo "Please, specify a message file!"
    exit 1
fi
file="$(realpath $1)"

# Create an output folder
mkdir -p out

for stage in $(ls -1 stages/)
do
    echo "Running stage $stage..."
    output="$(realpath out/stage_${stage})"
    pushd stages/${stage} > /dev/null
    ./pack.sh ${file} ${output}
    popd > /dev/null
    file="${output}"
done

echo "The gift is waiting at:"
echo $file