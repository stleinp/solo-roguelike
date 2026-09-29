# Devlog

Na elke werksessie een korte notitie: wat gedaan, wat geleerd, welke mijlpaal af, en —
vanaf M5 — het gemeten aantal steps/seconde. Nieuwste bovenaan. Doel: na twee weken
stilstand zonder aanloop verder kunnen, en expliciteren wat je geleerd hebt.

---

## 29 september 2026 (sessie 3) — Playtest geslaagd; fase 2b gepland

**Gedaan**
- Playtest door iemand anders geslaagd: fase 2 is af.
- Gevraagd: een mooiere grafische weergave en de vraag of 3D kan. Besloten (B23):
  simulatie blijft 2D; eerst een 2D-tilefrontend met raylib als fase 2b (mijlpalen
  G0–G3, ~3 weken tijdsbox), daarna beslissen over 3D-*weergave* (B24). Fase 3 volgt
  na G3.
- Docs bijgewerkt: `beslissingen.md` (B23, B22 aangepast), `projectplan.md` (fase 2b,
  scope, repostructuur), `README.md`, `CLAUDE.md`. Geen code gewijzigd.

**Geleerd / onthouden**
- 3D is twee vragen: een 3D-weergave van een 2D-grid past in de architectuur, een
  3D-simulatie niet. De echte kost van 3D zit in assets, niet in code.
- Dankzij B5 kost een tweede frontend niets aan `sim`; dat betaalt zich nu terug.

**G0 (venster) — af, door Stan visueel bevestigd**
- `input.hpp/.cpp` (`Key`, `to_action`) en `level.hpp` (kaart, `clock_seed`) uit de
  terminalversie gehaald, zodat beide frontends ze delen. `delve` gedrag ongewijzigd.
- `src/game/gfx_main.cpp` → `delve_gfx`: gekleurde vakjes, HUD, zelfde toetsen (q sluit,
  Escape niet). raylib 5.5 via `FetchContent`, alleen gelinkt aan `delve_gfx`;
  optie `DELVE_BUILD_GFX` om het uit te zetten.
- Bouwt zonder waarschuwingen in Debug (ASan/UBSan); 39 tests groen. Gestart in WSLg:
  X11-venster, OpenGL 4.5 via `llvmpipe` (software), 60 fps-doel, geen crash.
- `omgeving.md` bijgewerkt met de extra pakketten en WSLg-bevinding.

**Geleerd / onthouden**
- Raylib's GLFW vraagt `libxrandr-dev`, `libxinerama-dev`, `libxcursor-dev`, `libxi-dev`;
  zonder die faalt de *configure*, niet de build. Daarom de optie.
- WSLg geeft hier geen GPU-versnelling (`llvmpipe`). Voor 2D geen probleem.

**G1 (tiles en HUD) — af, door Stan visueel bevestigd**
- Assets: Kenney *Tiny Dungeon* (CC0, 16×16, packed sheet 12×11) in `assets/`, met
  licentietekst en een `README.md` met de gebruikte tegelnummers. CMake kopieert
  `assets/` bij het bouwen naast `delve_gfx`; het programma laadt via
  `GetApplicationDirectory()`, dus de werkmap maakt niet uit.
- `src/game/layout.hpp/.cpp`: pure functies (`sheet_rect`, `cell_rect`, `window_size`,
  `sprite_index`) zonder raylib, in de `render`-library. 5 nieuwe doctests (44 totaal).
- `gfx_main.cpp`: sprites (vloer eronder, karakters erbovenop), HUD met HP-balken
  voor speler en vijand, beurt, seed en toetsen, en een eindscherm over de kaart
  bij winst, verlies en timeout. Sprites 4× vergroot met point-filtering.

**Geleerd / onthouden**
- Zonder `unzip` en zonder PIL heb ik de sheet bekeken via een wegwerpprogrammaatje met
  raylib zelf (`ImageResizeNN` + raster). Handig om tegelnummers te kiezen.
- Tegel-indexen (muur 40, vloer 0, doel 45, speler 96, vijand 110) staan op één plek,
  `layout.cpp`; wisselen van sprite is een getal veranderen.

**G2 (beurtanimatie) — af, door Stan visueel bevestigd**
- `src/game/anim.hpp/.cpp`: pure functies zonder raylib (`make_delta`, `ease_out`,
  `lerp_cell`, `flash_strength`) met 6 doctests (50 totaal). Floats staan alleen hier
  en in `gfx_main.cpp`, nooit in `sim`.
- `gfx_main.cpp`: speler en vijand glijden 0,12 s naar hun nieuwe tegel; bij schade
  flitst de getroffen sprite rood (0,2 s); een gedode vijand vervaagt tijdens de
  beurt waarin hij sneuvelt. Het eindscherm verschijnt pas ná de laatste animatie.
- Invoer: één toets wordt gebufferd terwijl een animatie loopt (een nieuwere vervangt
  hem); `r` wist buffer en animatie. Zo gaan snelle toetsen niet verloren.
- `State` en `sim` zijn niet aangeraakt: de state bevat direct het resultaat, de
  animatie is alleen weergave (`TurnDelta` + timer).

**Geleerd / onthouden**
- `IsKeyPressed` levert een toets één keer; wie invoer tijdens de animatie negeert,
  verliest toetsen. Daarom de buffer van één.
- De HUD loopt een animatie voor (HP springt direct); voor nu bewust zo gelaten.
- Bump-to-attack heeft geen beweging, dus geen uitval-animatie; alleen de flits.

**G3 (afronden en 3D-beslispunt) — af**
- Playtest van de grafische versie geslaagd. Fase 2b is af, binnen de tijdsbox.
- B24: **3D nu niet, wel als mogelijke toekomst.** Redenen: fase 3 is de kern, 3D voegt
  bij één kaart en één vijand weinig toe, de kost zit in assets, en WSLg draait op
  `llvmpipe`. In B24 staat een schets van de overgang (spike van 2–3 uur in een aparte
  executable, wat hergebruikt wordt, wat niet verandert) en een heroverwegingsmoment
  (na fase 3, uiterlijk fase 6). Ook toegevoegd aan de lijst van fase 6.
- Docs bijgewerkt: `beslissingen.md` (B24), `projectplan.md`, `README.md`, `CLAUDE.md`.

**Geleerd / onthouden**
- Een tijdsbox werkte: G0–G2 in één sessie-reeks, geen scope-creep, en een expliciete
  beslissing over 3D in plaats van dat het stilletjes aansleept.
- Doordat `sim` nooit iets van weergave wist (B5), kostte een tweede frontend niets aan
  de kern, en zou een derde (3D) dat ook niet doen.

**Volgende:** fase 3. Begin met O2 (RL-stack): overleggen, klein beginnen, één omgeving,
één kaart. Nog niets gecommit sinds de laatste commit; het werk van fase 2 en 2b staat in
de werkboom.

---

## 29 september 2026 (sessie 2, vervolg) — Fase 2: speelbare terminalversie

**Gedaan**
- O3 beslist: ruwe ANSI/termios, geen bibliotheek (B22).
- `src/game/`: `render` (pure functie State → tekst, met/zonder kleur), `Terminal`
  (RAII raw mode + alternate screen, pijltjes/wasd/hjkl/spatie/r/q) en `main.cpp`
  (`delve [seed]`). Eén vaste level; `r` start een nieuw spel met een afgeleide seed.
- 5 renderertests (39 in totaal). Het programma zelf getest via een pseudo-terminal:
  starten, toetsen sturen, tussenstand lezen, afsluiten; de terminal wordt netjes
  hersteld. Zonder terminal geeft het een duidelijke melding.

**Geleerd / onthouden**
- `render` als pure functie maakt de UI testbaar zonder terminal; alleen de dunne
  `Terminal`-laag raakt het OS.
- Ctrl-C als toets lezen (ISIG uit) garandeert dat de destructor de terminal herstelt.
- De klok mag in `game` (seed), niet in `sim`. Layering werkt: `sim` is niet aangeraakt.

**Nog te doen voor "klaar" van fase 2:** iemand anders laten spelen zonder uitleg en
noteren waar die vastloopt. Ik heb zelf alleen via een pty getest, niet als mens.

**Volgende:** playtest afronden; daarna fase 3 (O2 RL-stack).

---

## 29 september 2026 (sessie 2, vervolg) — M5 benchmark af, fase 1 klaar

**Gedaan**
- `tools/bench/main.cpp`: speelt willekeurige episodes op de vaste 11×7-kaart (elke
  episode een eigen seed), telt uitkomsten en stappen en print steps/s plus een checksum
  (XOR van de eindhashes). Waarschuwt in een build zonder `NDEBUG`.
- Meting (Release, g++ 13.3, één thread, 200.000 episodes, 3 runs):
  **~49 miljoen steps/s** (49,1–49,3 M); ~11,8 stappen per episode.
  Uitkomsten: 0,7 % gewonnen, 55 % verloren, 44 % timeout.
- Checksum is identiek bij elke run: ook de benchmark is deterministisch.

**Geleerd / onthouden**
- Fase 3 wordt door de simulatie niet beperkt: 49 M steps/s is ver boven de
  500.000 uit het projectplan. Het knelpunt wordt de taalgrens en de training.
- "Reset" is hier een kopie van de initiële `State` (één `vector`-allocatie); als dat
  ooit zwaar weegt, is een reset-in-place mogelijk. Nu niet nodig.
- Het getal is een startpunt. Het gebruik van een willekeurige policy maakt episodes
  kort; een slimmere agent speelt langere episodes met andere kosten per stap.
- De eerste meting van 0,05 s was te kort; 200.000 episodes geeft stabielere getallen.

**Fase 1 is klaar** volgens de criteria: duizenden willekeurige episodes zonder
rendering, determinismetest slaagt, en er is een getal voor throughput.

**Volgende:** fase 2 — beslis O3 (rendering) en maak het speelbaar.

---

## 29 september 2026 (sessie 2, vervolg) — M4 determinismetest af

**Gedaan**
- `src/sim/hash.hpp/.cpp`: FNV-1a over alle `State`-velden (B21).
- `tests/test_determinism.cpp`: 100 seeds twee keer gespeeld en vergeleken; andere seed
  of andere acties geeft andere hash; hash reageert op elk veld; golden waarde
  `0x19936cd070a5f202` (seed 12345, actieseed 67890, 200 stappen).
- 34 tests groen in Debug (ASan/UBSan); de golden klopt ook in Release, dus de uitkomst
  hangt niet af van optimalisatie.

**Geleerd / onthouden**
- De golden waarde heb ik uit de huidige implementatie gehaald: hij vangt regressies,
  niet fouten. De run eindigt in `Lost` op beurt 34.
- Acties uit een aparte RNG-stroom houden de test onafhankelijk van de game-RNG.

**Volgende:** M5 — `bench` in Release, steps/seconde in de devlog.

---

## 29 september 2026 (sessie 2, vervolg) — M3 RNG en vijand af

**Gedaan**
- `src/sim/rng.hpp/.cpp`: splitmix64, xoshiro256**, `range(n)` zonder bias. Seed via
  `parse_map(text, seed)`. Beslissingen in B20.
- Vijand achtervolgt of dwaalt (1 op 4), slaat als hij naast de speler staat.
- 11 nieuwe tests (29 in totaal): RNG (referentiewaarde splitmix64, uniformiteit,
  replay van een gekopieerde state) en vijand (invarianten over 50 seeds, corridor,
  zelfde seed = zelfde spel, andere seed = ander pad). Groen met ASan/UBSan.

**Geleerd / onthouden**
- Rejection sampling: de eerste `2^64 mod n` waarden weggooien geeft een exact uniforme
  `range`; `(0 - n) % n` in unsigned rekenen is die drempel.
- De vijand kan achter een muur blijven vastzitten (geen pathfinding). Bewust; kijk er
  opnieuw naar als de agent er last van krijgt.

**Volgende:** M4 — hash van de eindtoestand en golden-waarde.

---

## 29 september 2026 (sessie 2, vervolg) — M2 `step()` af

**Gedaan**
- `src/sim/step.hpp/.cpp`: `StepResult step(State&, Action)`, `Outcome`, `outcome_of`.
  Regels vastgelegd in B19.
- 13 nieuwe tests, één of twee per regel (18 in totaal); alles groen met ASan/UBSan.
- De vijand slaat al als hij naast de speler staat, zodat `Lost` in M2 testbaar is;
  bewegen en RNG volgen in M3.

**Geleerd / onthouden**
- Uitkomst afleiden uit de state ipv een vlag bijhouden houdt `State` de enige bron van
  waarheid, wat later ook de determinismehash (M4) eenvoudiger maakt.

**Volgende:** M3 — eigen RNG en vijandgedrag.

---

## 29 september 2026 (sessie 2, vervolg) — M1 datamodel af

**Gedaan**
- `src/sim/types.hpp` (`Action`, `Tile`, `Pos`), `state.hpp/.cpp` (`State`,
  `parse_map`). Kaartformaat: `#` muur, `.` vloer, `>` doel, `@` speler, `E` vijand.
- `parse_map` valideert: rechthoekig, precies één speler en vijand, minstens één doel.
  Geeft `std::optional<State>`.
- 5 tests op inlezen en grenzen; alles groen met ASan/UBSan.
- B18: vijf acties met bump-to-attack.

**Geleerd / onthouden**
- `rng_state` staat al in `State` als lege array; de echte RNG komt in M3.
- Foutmeldingen bij ongeldige kaarten zijn bewust weggelaten (`nullopt`); pas toevoegen
  als het debuggen erom vraagt.

**Volgende:** M2 — `step()`.

---

## 29 september 2026 (sessie 2) — M0 skelet af

**Gedaan**
- Toolchain gecontroleerd; `ninja-build` en `clang-format` bijgeïnstalleerd.
- Skelet: `CMakeLists.txt`, `CMakePresets.json` (debug/release), `src/sim/` (lege
  library met placeholder `sim::version()`), `tests/` met doctest via FetchContent,
  `.gitignore`, `.clang-format`. Warnings aan; ASan/UBSan alleen in Debug, via een
  `delve_options` interface-library zodat doctest niet aan onze warnings hoeft te voldoen.
- Debug + Release bouwen; `ctest` slaagt (1/1).
- B17 vastgelegd: code Engels, docs Nederlands. Paden in docs rechtgezet
  (`~/solo-roguelike`).

**Geleerd**
- `sim` linkt niets en de opties zijn `PRIVATE`: consumenten erven geen sanitizers
  via `sim`, maar linken ze zelf via `delve_options`. Beide kanten moeten dezelfde
  sanitizers hebben, anders klopt het niet.

**Volgende:** M1 — datamodel; beslis 5 of 6 acties.

---

## 29 september 2026 — Planning afgerond, taal en omgeving gekozen

**Gedaan**
- Brainstorm uitgewerkt tot projectplan, beslissingslogboek, brainstormverslag en
  leeslijst.
- Documentatie verplaatst naar `docs/`; links in de README rechtgezet.
- Taal gekozen: **C++20** ([B10](beslissingen.md#b10--c20-als-implementatietaal-was-o1)).
  C en C++ zijn het best gekend; nieuwe taal + nieuw domein + nieuwe toolchain zou te
  veel tegelijk zijn.
- Omgeving gekozen: **WSL2 Ubuntu 24.04 + VS Code** (B11), met **CMake + Ninja,
  doctest, sanitizers** (B12). Doorslaggevend: de Python-binding in fase 3 en
  RL-tooling werken op Linux zonder gedoe; op Windows botst MinGW met de MSVC-Python.
- Codestijl (B13), determinismeregels (B14), "Python pas vanaf fase 3" (B15) en de
  MVP-definitie (B16) vastgelegd.
- Fase 1 opgedeeld in mijlpalen M0–M5 (zie projectplan).
- `CLAUDE.md` en `docs/omgeving.md` toegevoegd zodat een nieuwe sessie in WSL meteen
  mee is.

**Geleerd / onthouden**
- `std::uniform_int_distribution` is niet portabel deterministisch — dezelfde seed kan
  per standaardbibliotheek andere getallen geven. Eigen RNG schrijven.
- Een Python-extensie bouwen met MinGW tegen een MSVC-gebouwde Python geeft
  ABI-problemen. Nog een reden voor Linux.
- De scheiding sim/render (B5) kun je laten afdwingen door de build: `sim` als eigen
  CMake-library die tegen niets linkt.

**Losse eindjes** (zie ook `CLAUDE.md`)
- Aantal acties vastleggen bij M1: vijf (bump-to-attack) of zes (expliciete aanval).
- B2 noemt "real-time loops" en "performance onder een frame budget" als leerwinst,
  terwijl real-time buiten scope valt; de echte performance-uitdaging hier is
  simulatie-throughput. Formulering eventueel aanpassen.
- In `brainstorm.md` verwijst de bron voor het UGent@Work-planlastonderzoek naar een
  partijwebsite in plaats van naar het onderzoek zelf. Alleen relevant als het
  edtech-idee ooit terugkomt.

**Volgende sessie:** repo klonen in WSL, toolchain installeren (`omgeving.md`), dan M0.
