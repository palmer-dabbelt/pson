#include "_tempdir.bash"

cat >$INPUT <<"EOF"
"simple string",
EOF

cat >$OUTPUT.gold <<"EOF"
"simple string"
EOF

#include "_harness.bash"
