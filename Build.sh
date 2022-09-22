#!/bin/sh
git submodule update --init --recursive
export VULKAN_SDK=$HOME/VulkanSDK/1.3.224.1/macos
python3 ./Buildster/buildster/buildster.py ./Project.xml

