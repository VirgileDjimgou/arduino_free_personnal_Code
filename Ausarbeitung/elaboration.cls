% Erstellt von Ulrich Klauck, überarbeitet von Dominik Bergen

\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{elaboration}[2016/06/22 Ausarbeitung]

\PassOptionsToClass{
	12pt,		% Schriftgröße [10pt, 11pt, 12pt]
	a4paper,	% Papierformat [a4paper, a5paper, b5paper, letterpaper, legalpaper]
	onecolumn,	% Ein- oder zweispaltig [onecolumn, twocolumn]
	final,		% Entwurf oder finale Version [draft, final]
	ngerman,	% Neue deutsche Rechtschreibung
	reqno,		% Position der Nummerierung bei Formeln [leqno, reqno]
	fleqn,		% Linksbündige Formeln [fleqn]
	twoside,	% Ein- oder zweiseitig [oneside, twoside]
	openright,	% Position eines Kapitelanfangs [openright, openany]
	parskip=2ex,
	headings=big
}{scrbook}

\DeclareOption*{
	\PassOptionsToClass{\CurrentOption}{scrbook}
}

\ProcessOptions\relax
\LoadClassWithOptions{scrbook}

% Diverse Pakete
\RequirePackage[dvipsnames]{xcolor}
\RequirePackage{ucs}
\RequirePackage[utf8x]{inputenc}
\RequirePackage[T1]{fontenc}
\RequirePackage[ngerman]{babel}
\RequirePackage[babel, german=quotes]{csquotes}
\RequirePackage{lmodern}
\RequirePackage[automark]{scrpage2}
\RequirePackage{calc}
\RequirePackage{array}
\RequirePackage{newcent}
\RequirePackage{setspace}  
\RequirePackage{blindtext}
\RequirePackage{placeins}
\RequirePackage{float}
\RequirePackage[final,hidelinks]{hyperref}
\RequirePackage[ngerman]{cleveref}
\RequirePackage[font=small, labelfont=bf, format=plain, justification=centering]{caption}
\usepackage{wrapfig}
\usepackage{mdframed}
\fboxsep0mm

% Grafik
\RequirePackage{graphics}
\RequirePackage{graphicx}
\RequirePackage{pgf}
\RequirePackage{caption}[2008/08/24]
\RequirePackage{subfigure}

% Mathematik
\RequirePackage{amsmath}
\RequirePackage{amssymb}
\RequirePackage{amstext}
\RequirePackage{amsthm}
\RequirePackage{mathtools}
\RequirePackage{nicefrac}
\RequirePackage{siunitx}

% Algorithmen und Quellcode
\RequirePackage{scrhack}
\RequirePackage[formats]{listings}
\RequirePackage{algorithm}
\RequirePackage{algpseudocode}
\RequirePackage{struktex}
\RequirePackage{filecontents}
\RequirePackage{forloop}
\RequirePackage{ifthen}
\RequirePackage{cancel}

% Tabellen
\RequirePackage{longtable}
\RequirePackage{tabularx}
\RequirePackage{multirow}
\RequirePackage{multicol}
\RequirePackage{booktabs}

% Diverse Symbole
\RequirePackage{textcomp}
\RequirePackage{gensymb}
\RequirePackage{marvosym}

% Indizes
\RequirePackage{makeidx}\makeindex

% Seitenlayout
\pagestyle{scrheadings}
\clearscrheadfoot
\ihead{\headmark}
\ohead{\pagemark}
\raggedbottom
\usepackage[a4paper, total={155mm,245mm}]{geometry}

% Literaturverzeichnis
\RequirePackage[numbers,round]{natbib}

% Serifenlose Schrift
\renewcommand*\familydefault{\sfdefault}

% Programmiersprachen
\lstloadlanguages{R,Java,Python,[ISO]C++,[ANSI]C++,[ANSI]C}

\lstset{
	inputencoding=utf8x,
	extendedchars=\true,
	numbers=left,
	tabsize=3,
	stepnumber=1,
	captionpos=b,
	breaklines=true,
	breakatwhitespace=true,
	xleftmargin=4ex,
	xrightmargin=0ex,
	showtabs=false,
	showspaces=false,
	showstringspaces=false,
	basicstyle=\ttfamily\color{black}\small,
	keywordstyle=\ttfamily\color{blue},
	stringstyle=\ttfamily\color{green},
	commentstyle=\ttfamily\color{orange},
	numberstyle=\ttfamily\color{gray},
	identifierstyle=\ttfamily\color{black},
	morekeywords={},
	literate=
		{Ö}{{\"O}}1 
		{Ä}{{\"A}}1 
		{Ü}{{\"U}}1 
		{ß}{{\ss}}2 
		{ü}{{\"u}}1 
		{ä}{{\"a}}1 
		{ö}{{\"o}}1
}

\lstdefinelanguage{R} {
	morekeywords=[1]{\<<-},
	sensitive=false,
	morecomment=[l]{\#},
	morecomment=[s]{/*}{*/},
	morestring=[b]",
}[keywords,comments,strings]

\lstdefinelanguage{CSS} {
	morekeywords={property},
	sensitive=false,
	morecomment=[l]{//},
	morecomment=[s]{/*}{*/},
	morestring=[b]",
}

\lstdefinelanguage{C} {
	sensitive=false,
	morecomment=[l]{//},
	morecomment=[s]{/*}{*/},
	morestring=[b]"
}

% Fehlerausgabe
\setcounter{errorcontextlines}{5}
\setlength\parskip{2ex}

% Deckblatt
\def\@depurl{https://www.hs-aalen.de/courses/68}
\def\depurl#1{\def\@depurl{#1}}

\def\@depname{Informatik}
\def\depname#1{\def\@depname{#1}}

\newcommand{\departmentaddress}{Informatik}
\newcommand{\depuniversity}{Hochschule Aalen}

\def\doctype#1{\def\@doctype{#1}}
\def\examinerA#1{\def\@examinerA{#1}}
\def\examinerB#1{\def\@examinerB{#1}}
\def\@datetext{Einreichungsdatum:}
\def\datetext#1{\def\@datetext{#1}}
\renewcommand\maketitle{
	\begin{titlepage}
		\sffamily
		\begin{flushright}
			\includegraphics[width=.75\textwidth]{images/htw-aalen.pdf}
		\end{flushright}
		\vskip 2cm
		\begin{flushleft}
			\par \large \color{gray}
			\ifx\@doctype\undefined
			\else
				\@doctype \\
				Studiengang\xspace\@depname\xspace\\[0ex]
			\fi
			
			\par \rule{\textwidth}{0.25pt}
			\ifx\@author\undefined
			\else
				\par \Large \color{black}
				\@author
			\fi
		\end{flushleft}
		
		\bigskip
		\bigskip
		\bigskip
		
		\begin{center}
			\ifx\@title\undefined
			\else
				\par\Huge\@title
			\fi
		\end{center}
		
		\vfill
	
		\begin{tabular}{ll}
			\ifx\@examinerA\undefined
			\else
				Prüfer:		& \@examinerA \\
			\fi
			
			\ifx\@examinerB\undefined
			\else
				& \@examinerB \\
			\fi
			\ifx\@date\undefined
			\else
				& \\
				\@datetext	& \@date \\
			\fi
		\end{tabular}
	\end{titlepage}
}

% Thick horizontal and vertical lines for tables
\newcommand{\thickhline}{
    \noalign {\ifnum 0=`}\fi \hrule height 1.5pt
    \futurelet \reserved@a \@xhline
}
\newcolumntype{=}{@{\hskip\tabcolsep\vrule width 1.5pt\hskip\tabcolsep}}

\selectlanguage{ngerman}
