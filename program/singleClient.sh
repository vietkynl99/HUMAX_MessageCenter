#!/bin/bash

./msg_client << EOF
127.0.0.1
$1
3
'hello from script'
:q
EOF