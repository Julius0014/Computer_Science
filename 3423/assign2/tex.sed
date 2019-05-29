s/\\/\\verb+\\+/g
s/%/\\%/g
s/--/-\\hspace{.01cm}-/g
s/^\([A-Z]\{4,\}\)/\\item[\1] \\hfill \\ /g
1 i\
\\documentstyle[11pt]{article}
1 i\
\\begin{document}
$ a\
\\end{description}
1 i\
\\begin{center} {\bf
1 a\
} \\end{center}
1 a\
\\begin{description}
$ a\
\\end{document}
/^+\|^-/s/$/ \\\\/g
