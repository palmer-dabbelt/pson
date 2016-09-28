#include "_tempdir.bash"

cat >$INPUT <<"EOF"
[
  "string 1",
  "string 2"
]
EOF

cat >$OUTPUT.gold <<"EOF"
[
  "string 1",
  "string 2"
]
EOF

#include "_harness.bash"
