#!/bin/sh
python3 -m pip install buildster
export VULKAN_SDK=$HOME/VulkanSDK/1.3.224.1/macos
buildster ./Project.xml

