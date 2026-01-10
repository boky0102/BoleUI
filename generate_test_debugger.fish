#!/usr/bin/env fish

set projectPath $argv[1]
set testFile $argv[2]
set line $argv[3]

echo $projectPath
echo $testFile
echo $line

set testname (deno --allow-read /home/bole/dev/scripts/testname/main.ts $testFile $line)

cd $projectPath/build-debug
ctest --show-only=json-v1 | deno run /home/bole/dev/scripts/rte/main.ts $testname --debug-info > $projectPath/.zed/debug.json
