SUBDIRS = FeLib Main Script Graphics

EXTRA_DIST = IVAN.dsw FeLib.dsp Main.dsp ivandj.mak ivanmgw.mak LICENSING

install-data-local:
	-./mkinstalldirs $(localstatedir) $(bindir)
	-touch $(localstatedir)/ivan-highscore.scores
	-chown games.games $(localstatedir)/ivan-highscore.scores
	-chmod 664 $(localstatedir)/ivan-highscore.scores
	-chgrp games $(bindir)/ivan 
	-chmod 2111 $(bindir)/ivan
