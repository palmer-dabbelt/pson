#include "_tempdir.bash"

cat >$INPUT <<"EOF"
"simple string"
EOF

cp $INPUT $OUTPUT.gold

#include "_harness.bash"
