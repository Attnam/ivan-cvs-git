SUBDIRS = FEEL FeWin FeFile FELL FeDX FeMath FeIO LibTest Main Script Graphics

EXTRA_dist = FEEL.dsp FeFile.dsp FeIO.dsp IVAN.dsw FELL.dsp FeMath.dsp FeWin.dsp Main.dsp FeDX.dsp LibTest.dsp winmakefile Profdeb.bat Profrel.bat

install-data-local:
	touch /var/lib/games/ivan-highscores.scores
	-chown games.games /var/lib/games/ivan-highscores.scores
	-chmod 664 /var/lib/games/ivan-highscores.scores
	-chgrp games $(DESTDIR)$(bindir)/ivan && chmod 2111 $(DESTDIR)$(bindir)/ivan
