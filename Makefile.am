SUBDIRS = FeLib Main Script Graphics

EXTRA_DIST = IVAN.dsw FeLib.dsb Main.dsp Graphics/Font.pcx Graphics/FOW.pcx Graphics/Char.pcx Graphics/Human.pcx Graphics/Cursor.pcx Graphics/Item.pcx Graphics/LTerrain.pcx Graphics/Symbol.pcx Graphics/WTerrain.pcx Script/dungeon.dat ivandj.mak ivanmgw.mak

install-data-local:
	touch /var/lib/games/ivan-highscores.scores
	-chown games.games /var/lib/games/ivan-highscores.scores
	-chmod 664 /var/lib/games/ivan-highscores.scores
	-chgrp games $(DESTDIR)$(bindir)/ivan && chmod 2111 $(DESTDIR)$(bindir)/ivan
