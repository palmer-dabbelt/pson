#include "_tempdir.bash"

cat >$INPUT <<"EOF"
[
  1,
  2,
  3,
  5,
  3
]
EOF

cp $INPUT $OUTPUT.gold

#include "_harness.bash"
