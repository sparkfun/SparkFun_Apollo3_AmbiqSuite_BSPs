This folder contains original (or close to original) scripts from the AmbiqSuite SDK all rolled into one easy to use item.

Usage: ```python ambiq_bin2board.py --bin ${build.path}/${build.project_name}.bin --load-address-blob 0x20000 --magic-num 0xCB -o ${build.path}/${build.project_name} --version 0x0 --load-address-wired 0xC000 -i 6 --options 0x1 -b ${baud_rate} -port ${port} -r 2 [-v]```

Previous method:
1. Created signed OTA blob
2. Wrap OTA blob in wired blob
3. Upload signed blobs via UART
