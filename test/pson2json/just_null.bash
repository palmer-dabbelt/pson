#include "_tempdir.bash"

cat >$INPUT <<"EOF"
null
EOF

cp $INPUT $OUTPUT.gold

#include "_harness.bash"
