s/\\/\\verb+\\+/g
s/%/\\\%/g
s/\^/\\\^/g
s/--/-\\hspace{.01cm}-/g
s/^\([A-Z][A-Z]*\)$/\\item[\1] \\hfill \\\\/g
/^[ 	][ 	]*-./ s/$/ \\\\/g
/^[ 	][ 	]*+./ s/$/ \\\\/g
1 i \\\documentstyle[11pt]{article}
1 i \\\begin{document}
1 i \\\begin{center} {\\\bf
2 i \\} \\end{center}
2 i \\\begin{description}
$ a \\\end{description}
$ a \\\end{document}
