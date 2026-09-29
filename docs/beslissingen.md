# Beslissingslogboek

Elke beslissing met de reden erbij, zodat later-jij weet waarom iets zo is en niet
opnieuw dezelfde discussie voert. Beslissingen mogen omgegooid worden — noteer dan
waarom.

Laatst bijgewerkt: 29 september 2026 (B24)

---

## Vastgelegd

### B1 — Het doel is leren, niet geld

**Beslissing:** het project wordt beoordeeld op wat het je leert, niet op wat het
opbrengt.

**Waarom:** de uitgangsvraag was aanvankelijk "iets dat later geld kan opbrengen", maar
bij nader inzien is het werkelijke motief technisch scherp blijven tijdens een opleiding
die inhoudelijk elders ligt. Dat zijn verschillende projecten met verschillende
optimalisaties.

**Gevolg:** marktomvang, monetisatie en doelgroep zijn geen selectiecriteria meer. De
faalmodus verschuift van "niemand wil dit" naar twee andere: iets bouwen dat je al kunt,
of nooit bij de laatste 20% komen. Daar is de fasering op ontworpen.

---

### B2 — Een game, geen SaaS

**Beslissing:** een game, niet een softwareproduct voor een niche.

**Waarom:** de leer-dichtheid per uur ligt hoger. Real-time loops, rendering, performance
onder een frame budget, systemen die op elkaar inwerken, wiskunde — dat zie je niet in
een gewone applicatiestack. Motivatie is bovendien de schaarse grondstof bij een
sideproject naast een opleiding: een project dat je leuk vindt maak je af, een
verstandig project laat je in februari vallen.

**Alternatieven verworpen:** zie [`brainstorm.md`](brainstorm.md). De SaaS-ideeën zijn
niet slecht, ze passen alleen bij een ander doel.

---

### B3 — Eigen engine, geen Unity of Godot

**Beslissing:** simulatie en renderer worden zelf geschreven.

**Waarom:** dit is geen esthetische keuze maar een technische vereiste. RL heeft nodig:
volledig deterministische simulatie, een headless modus zonder rendering, en de
mogelijkheid om duizenden kopieën parallel op maximale snelheid te draaien. Kant-en-klare
engines geven je die drie dingen niet of alleen met veel pijn.

**Bonus:** dezelfde eigenschappen leveren replays, save/load en deterministische tests
gratis op.

**Grens:** dit wordt *deze game's* engine, geen algemeen framework. Zie het
waarschuwingssignaal in [`projectplan.md`](projectplan.md#4-hoe-je-weet-dat-het-misgaat).

---

### B4 — Genre: turn-based grid roguelike

**Beslissing:** turn-based, grid-based, ASCII of eenvoudige tiles.

**Waarom:**
- Mechanisch rijk maar visueel eenvoudig — geen artiest nodig
- ASCII is in dit genre authentiek in plaats van een compromis
- Turn-based en discreet sluit natuurlijk aan op de RL-formulering
- Kleine, afgebakende stappen passen bij versnipperde werktijd

---

### B5 — Simulatie, rendering en input strikt gescheiden

**Beslissing:** de simulatie weet niets van rendering of input. Vanaf regel één.

**Waarom:** dit is de enige beslissing die nu al goed moet. Als rendering in de game
logic verweven raakt, kan er in fase 3 niet meer headless getraind worden zonder alles
te herschrijven. Alle andere keuzes zijn later nog om te gooien.

**Hoe afgedwongen:** via aparte CMake-targets, zie B12.

---

### B6 — Deterministisch en seeded vanaf het begin

**Beslissing:** zelfde seed plus zelfde acties geeft altijd dezelfde eindtoestand. Er
komt een test die dat bewijst, in fase 1.

**Waarom:** reproduceerbare training, bruikbare bugreports, replays, en de mogelijkheid
om een run exact opnieuw af te spelen. Achteraf determinisme inbouwen is veel duurder
dan het vanaf het begin afdwingen.

**Concrete regels:** zie B14.

---

### B7 — Klein beginnen, pas uitbreiden als het werkt

**Beslissing:** eerste trainbare versie is volledig observeerbaar, met een kleine
actieruimte (vier richtingen, aanvallen, wachten) en een vaste map.

**Waarom:** roguelikes combineren alles wat RL moeilijk maakt: procedurele generatie,
sparse reward, lange horizon, partiële observeerbaarheid, grote actieruimte. De
NetHack Learning Environment bestaat als onderzoeksbenchmark precies daarom, en
publicaties daar trainen met miljarden environment steps. Dat budget is er hier niet.
Fog of war dwingt bovendien naar recurrente policies — te vroeg.

**Gevolg:** curriculum learning is het vangnet. 5×5 lege kamer → één vijand → gangen →
grotere map.

---

### B8 — Agents zijn meetinstrument, geen tegenstander

**Beslissing:** getrainde vijanden zijn géén fundament van het project. De agent wordt
gebruikt om levels te evalueren.

**Waarom:** het oorspronkelijke idee was om ook vijanden via RL te trainen voor de
moeilijkheidsgraad. Drie bezwaren:

1. Games gebruiken behavior trees, utility AI en GOAP niet uit achterstand maar omdat
   ontwerpers controle en leesbaarheid nodig hebben. RL-vijanden zijn onvoorspelbaar,
   moeilijk te tunen en vinden degenererende exploits.
2. Het doel van vijand-AI is niet winnen maar interessant verliezen. Een onverslaanbare
   vijand maak je in vijf regels code; de moeilijkheid zit in *leuk*, niet in *sterk*.
3. Speler en vijanden tegelijk laten leren is self-play met een niet-stationair
   doelwit. Dat is onderzoekswerk, geen startpunt.

**In plaats daarvan:** genereer een level, laat de agent er honderden keren doorheen
lopen, en gebruik win rate, gemiddeld aantal beurten en resterende HP als objectief
moeilijkheidslabel. Daarmee kun je onspeelbare en triviale levels wegfilteren, generatie
sturen op een doelmoeilijkheid, en balanswijzigingen meten in plaats van gokken.

Dit is bestaande, gebruikte techniek (agent-based evaluatie van procedureel gegenereerde
content), origineler dan getrainde vijanden, en wél haalbaar.

**Niet definitief afgeschreven:** geleerde vijanden mogen terugkomen in fase 6, als
experiment.

---

### B9 — Elke fase is op zichzelf af

**Beslissing:** het project wordt zo gefaseerd dat stoppen na elke fase iets bruikbaars
oplevert.

**Waarom:** engine, game en RL zijn elk op zich een jaar werk. Gestapeld is dat het
klassieke recept om nooit iets af te maken. De laatste 20% van een project bevat
bovendien het grootste deel van het leereffect: edge cases, performance, refactoren
terwijl er al gewicht op de structuur staat.

---

### B10 — C++20 als implementatietaal (was O1)

**Beslissing:** de simulatie, game en tools worden geschreven in modern C++ (C++20).
Beslist op 29 september 2026.

**Waarom:** C en C++ zijn de talen die het best gekend zijn. Volgens de werkregel
"verander één of twee dingen tegelijk" is een vertrouwde taal hier verstandig:
engine-architectuur, determinisme, performance en RL zijn al nieuw genoeg. De leerwinst
zit in het domein, niet in de syntax. C++ verkiezen boven C omdat:
- de Python-binding in fase 3 eenvoudiger is via nanobind of pybind11;
- `std::vector`, `std::array`, `enum class`, RAII en een standaardbibliotheek de kans
  op geheugenfouten verkleinen zonder performance te kosten.

**Hoe C++ hier leerzaam blijft:** bewust *modern en data-georiënteerd* schrijven —
zie B13. Niet het C++ van klassenhiërarchieën en `virtual`.

**Aandachtspunt voor O2:** sommige RL-tooling (PufferLib) ondersteunt native
environments in C. Een C++-simulatie kan indien nodig een dunne C-API (`extern "C"`)
aanbieden; dat is geen reden om de hele kern in C te schrijven.

**Verworpen:** Rust (grootste leerwinst, maar nieuwe taal + nieuw domein + nieuwe
toolchain is te veel tegelijk), C (meer voetangels, geen voordeel dat C++ niet ook
biedt), Zig (klein ecosysteem).

---

### B11 — Ontwikkelen in WSL2 (Ubuntu 24.04) met VS Code

**Beslissing:** alle ontwikkeling gebeurt in WSL2 (Ubuntu 24.04, al geïnstalleerd), met
VS Code via de Remote-WSL-extensie. De repo staat in het Linux-bestandssysteem
(`~/projects/solo-roguelike`), **niet** onder `/mnt/c/...`. Volledige Visual Studio is
niet nodig. Installatiestappen: zie [`omgeving.md`](omgeving.md).

**Waarom:**
1. *Fase 3 is op native Windows pijnlijk.* De Windows-Python (Miniconda) is met MSVC
   gebouwd; een Python-extensie compileren met de aanwezige MinGW-g++ geeft
   ABI-problemen. Op Windows zou je alsnog de MSVC Build Tools nodig hebben. Op Linux
   werkt het gewoon.
2. *RL-tooling is primair op Linux gericht* (PufferLib). CUDA werkt in WSL2 met de
   RTX 3050.
3. *Sanitizers* (`-fsanitize=address,undefined`) werken onder Linux, niet onder MinGW.
   Ze vangen geheugenfouten meteen in plaats van na drie avonden zoeken.
4. *Terminalrendering* voor fase 2 (ncurses) is op Linux standaard.
5. De aanwezige Windows-g++ komt mee met Strawberry Perl — een toevallige installatie,
   geen toolchain om een project op te bouwen.
6. De repo onder `/mnt/c` zetten maakt builds en git merkbaar trager.

**Verworpen alternatief:** fase 1–2 native op Windows met MinGW en pas in fase 3
overstappen. Haalbaar, maar dan betaal je de overstap in de zwaarste fase.

---

### B12 — Build en tooling: CMake + Ninja, doctest, sanitizers

**Beslissing:**
- **Build:** CMake (≥ 3.25) met Ninja als generator. Twee configuraties:
  `Debug` (met AddressSanitizer + UndefinedBehaviorSanitizer) en `Release` (`-O2`/`-O3`,
  voor de benchmark).
- **Compiler:** GCC (g++) in WSL; clang als tweede compiler is welkom.
- **Warnings:** `-Wall -Wextra -Wpedantic -Wshadow -Wconversion`, zonder ze te negeren.
- **Tests:** doctest (single header, snel compileren), binnengehaald via CMake
  `FetchContent`. Uitvoeren met `ctest`.
- **Formattering:** `.clang-format` in de repo; clangd of de C/C++-extensie in VS Code.
- **Line endings:** `.gitattributes` met `eol=lf`, zodat Windows- en WSL-checkouts
  hetzelfde zien.

**B5 afdwingen via de build:** de simulatie is een eigen CMake-library `sim` die tegen
niets linkt en geen I/O doet (geen `<iostream>`, geen bestanden, geen klok). Game,
benchmark en tests linken tégen `sim`, nooit omgekeerd. Als rendering in de simulatie
sluipt, faalt de build — de scheiding hangt dan niet af van discipline.

**Waarom:** standaard, goed ondersteund in VS Code (CMake Tools), en precies genoeg.
Geen eigen buildsysteem, geen package manager (vcpkg/conan) tot er een echte
afhankelijkheid is die dat rechtvaardigt.

---

### B13 — Data-georiënteerde codestijl

**Beslissing:** de game state is platte data: structs, `std::array`/`std::vector`,
`enum class` voor acties en tegeltypes, vrije functies zoals
`StepResult step(State&, Action)`. Geen entiteitshiërarchie met `virtual`, geen
heap-objecten per entiteit, geen pointers tussen entiteiten (gebruik indices).

**Waarom:** platte state is triviaal te kopiëren, te resetten, te hashen (determinisme-
test) en te vergelijken. In fase 3 moeten duizenden states parallel in een batch leven
en observaties naar een vooraf gealloceerde buffer geschreven worden; dat is met platte
data vanzelfsprekend en met een objectgraaf een herschrijving.

**Grens:** dit is geen ECS-framework. Zie het waarschuwingssignaal "de engine wordt
algemeen" in het projectplan.

---

### B14 — Concrete determinismeregels (invulling van B6)

**Beslissing:** in de `sim`-library gelden deze regels:
1. **Eigen RNG**, niet `<random>`'s distributies. `std::uniform_int_distribution` en
   verwanten zijn implementatie-afhankelijk: dezelfde seed geeft op een andere compiler
   of standaardbibliotheek andere getallen. Gebruik een eigen kleine generator
   (splitmix64 voor seeding, xoshiro256** of PCG voor de stroom) en een eigen
   `range(n)`-functie. De RNG-state zit ín de `State`, zodat kopiëren en herspelen
   exact werkt.
2. **Geen iteratie over ongeordende containers** (`std::unordered_map`/`_set`) waar de
   volgorde het resultaat beïnvloedt.
3. **Geen klok, geen tijd, geen globale of statische veranderlijke state.**
4. **Geen floats in de spellogica.** Alles op het grid is integer. (Floats mogen later in
   observaties voor de agent, buiten de spelregels.)
5. **Geen pointers of adressen als sleutel of sorteercriterium.**
6. **Vaste verwerkingsvolgorde** van entiteiten per beurt (bv. speler eerst, dan
   vijanden op index).

**Test:** seed + vaste actiereeks → hash van de eindtoestand, twee keer uitgevoerd én
vergeleken met een vastgelegde "golden" waarde. De golden waarde verandert alleen bewust,
met een devlog-notitie waarom.

---

### B15 — Python pas vanaf fase 3

**Beslissing:** fase 1 en 2 zijn puur C++. Er komt geen Python-omgeving tot fase 3.
Dan: één omgeving per project, ín WSL (Miniforge/conda of `uv`), vastgelegd in de repo
(`environment.yml` of `pyproject.toml`). De bestaande Windows-Miniconda is vanuit WSL
niet bruikbaar, en dat hoeft ook niet.

**Waarom:** nu Python erbij halen is twee onbekenden tegelijk debuggen — waarschuwings-
signaal 3 in het projectplan. De keuze conda vs. `uv` hangt af van de RL-stack (O2) en
wordt dan gemaakt.

---

### B16 — Wat "MVP" betekent in dit project

**Beslissing:** twee lagen.
- **MVP van de game** = einde fase 2: iemand anders kan het opstarten en spelen zonder
  uitleg.
- **MVP van het projectidee** = einde fase 3: een getrainde agent haalt op een vaste map
  betrouwbaar het doel, meetbaar beter dan willekeurig.

**Waarom:** "MVP" zonder definitie wordt een bewegend doel. Nu wordt er uitsluitend aan
fase 1 gewerkt, in kleine mijlpalen (M0–M5, zie projectplan).

---

### B17 — Taal van code en commentaar: Engels (was open punt bij M0)

**Beslissing:** identifiers en commentaar in de code zijn Engels; documentatie en
devlog blijven Nederlands.

**Waarom:** de C++-standaardbibliotheek, doctest, en later Python/RL-tooling zijn
Engelstalig; gemengde namen (`step` naast `beurtTeller`) lezen slecht. Documentatie is
voor de gebruiker zelf en blijft in de moedertaal.

---

### B18 — Vijf acties: bump-to-attack (beslist bij M1)

**Beslissing:** `Action` = Up, Down, Left, Right, Wait. Bewegen in de vijand is
aanvallen; er is geen aparte aanvalsactie.

**Waarom:** kleinere actieruimte voor de agent (B7), en het is de gebruikelijke
roguelike-conventie. Een aparte aanval voegt in een game zonder afstandswapens alleen
ongeldige acties toe. Dit lost de tegenstrijdigheid tussen projectplan ("vijf acties")
en B7 ("vier richtingen, aanvallen, wachten") op: B7 is bedoeld als "aanvallen via
bewegen".

**Gevolg:** als er later afstandsaanvallen of items komen, wordt de actieruimte dan pas
uitgebreid.

---

### B19 — Spelregels van `step()` (beslist bij M2)

**Beslissing:**
- Volgorde per beurt: speler, dan vijand (als die leeft), dan `turn++`.
- Tegen een muur of buiten het grid lopen doet niets, maar kost wel een beurt.
- Bewegen in de levende vijand = aanvallen (schade 1, vijand-HP 3); een dode vijand
  blokkeert niet meer.
- Op het doel stappen = direct `Won`; de vijand doet dan niets meer.
- Een vijand naast de speler slaat (schade 1, speler-HP 10); zie B20 voor bewegen.
- Einde: `Won`, `Lost` (speler-HP ≤ 0) of `Timeout` (200 beurten). `Lost` gaat voor
  `Won`, en die voor `Timeout`.
- De uitkomst wordt afgeleid uit de `State` (`outcome_of`), er is geen aparte
  "klaar"-vlag. `step()` op een afgelopen episode verandert niets.

**Waarom:** een geblokkeerde zet die gratis is, laat de agent zonder straf tegen muren
lopen; kosten per beurt houden het gedrag eerlijk. Een timeout is nodig voor RL
(episodes moeten eindigen) en voor de benchmark. Uitkomst afleiden uit de state
voorkomt dat vlag en state uit elkaar lopen. De getallen zijn constanten in
`step.hpp` en kunnen later bijgesteld worden bij balans.

---

### B20 — RNG en vijandgedrag (beslist bij M3)

**Beslissing:**
- RNG: xoshiro256** met splitmix64-seeding, eigen `range(n)` met rejection sampling
  (geen modulo-bias). De state (`RngState`, 4×`uint64_t`) zit in `State`. De seed komt
  binnen via `parse_map(text, seed)`.
- Vijand per beurt: naast de speler → slaan. Anders in 1 op 4 gevallen een willekeurige
  richting, anders een stap richting de speler (grootste afstandsas eerst, x bij
  gelijkspel; is die geblokkeerd, dan de andere as). Een geblokkeerde zet = blijven staan.
- De vijand betreedt geen muren, geen doeltegel en niet de tegel van de speler.

**Waarom:** een mix van achtervolgen en dwalen is voorspelbaar genoeg om te leren en
gebruikt de RNG, zodat determinisme (M4) echt iets bewijst. Een doeltegel blokkeren
voorkomt dat de vijand het doel bezet houdt, want bewegen in een vijand is aanvallen en
de speler zou het doel dan niet kunnen betreden. Pathfinding (BFS) is bewust niet gedaan:
de vijand kan achter een muur blijven hangen; dat is voor nu acceptabel en past bij
"klein beginnen" (B7).

---

### B21 — Determinismetest: FNV-1a over alle velden (beslist bij M4)

**Beslissing:** `hash(const State&)` is FNV-1a (64 bit) over álle velden van `State`
(afmetingen, tegels, posities, HP, beurt, RNG-state). Elk geheel getal gaat als vaste
little-endian bytes erin. De test speelt een vaste kaart met acties uit een aparte
RNG-stroom en vergelijkt (a) twee runs onderling over 100 seeds en (b) één run met een
vastgelegde golden waarde.

**Waarom:** byte voor byte hashen (niet `memcpy` van de struct) maakt de hash
onafhankelijk van padding en endianness. Acties uit een aparte RNG zorgen dat de test
niet meedraait op de RNG die hij test. Niet-cryptografisch is genoeg: we willen
regressies vangen, geen aanvallers.

**Let op:** de golden waarde bewijst geen correctheid, alleen dat het gedrag niet
onbedoeld verandert. Hij verandert bij elke wijziging aan regels, RNG-gebruik, `State`
of hash — dan bewust bijwerken en de reden in de devlog zetten. De huidige golden-run
eindigt met `Lost` op beurt 34; gebruik later ook een run die het doel haalt als de
regels uitbreiden.

---

### B22 — Rendering: ruwe terminal met ANSI, zonder bibliotheek (was O3)

**Beslissing:** de game tekent in de terminal met ANSI-escapecodes en leest toetsen via
`termios` (raw mode, alternate screen). Geen ncurses, SDL of raylib. Structuur in
`src/game/`:
- `render.hpp/.cpp` — `render(State, seed, color)` levert tekst op; een pure functie,
  dus testbaar zonder terminal.
- `terminal.hpp/.cpp` — RAII-klasse die raw mode aan- en uitzet en toetsen vertaalt.
- `main.cpp` — de lus: tekenen, toets lezen, `step()`.
`sim` weet niets van dit alles (B5); de afhankelijkheid loopt `game → sim`.

**Waarom:** het doel van fase 2 is dat iemand anders het kan opstarten zonder uitleg,
dus geen extra pakketten om te installeren. Dat het in WSL, Linux en macOS werkt is
genoeg. Rendering is hier een middel, geen leerdoel (zie de afweging bij O3); een
grafische renderer kan in fase 6. Ncurses was overkill voor een kaartje van 11×7.

**Details:** Ctrl-C wordt als toets (Quit) gelezen in plaats van als signaal, zodat de
destructor de terminal altijd herstelt. `delve <seed>` speelt een specifiek spel opnieuw;
de seed staat in beeld. De seed komt uit de klok: dat mag in `game`, niet in `sim`.
Zonder terminal (pipe) stopt het programma met een melding.

**Beperking:** Windows-native wordt niet ondersteund (termios); in WSL werkt het.

**Aangepast door B23:** de opmerking "een grafische renderer kan in fase 6" is achterhaald;
een 2D-grafische frontend komt vóór fase 3 (fase 2b). De terminalversie blijft bestaan.

---

### B23 — Grafische weergave: raylib, eerst 2D, 3D-beslissing later (fase 2b)

**Beslissing:**
- Vóór fase 3 komt een tijdsgebonden **fase 2b**: een 2D-grafische frontend met tiles,
  als tweede executable naast de terminalversie (mijlpalen G0–G3, zie projectplan).
- Bibliotheek: **raylib** (C, `FetchContent`, geen systeempakketten). Het venster draait
  in WSL2 via WSLg.
- De **simulatie blijft een 2D-integergrid.** Een 3D-simulatie (z-as, verdiepingen,
  physics) blijft buiten scope.
- **3D als weergave** van dezelfde 2D-`State` (blokken als muren, camera) is een optie
  na G3. Die keuze wordt pas genomen als de 2D-versie er staat (B24, nog te maken).
- Timebox: ongeveer 3 weken (~20 uur). Is G3 dan niet af, dan wordt de scope gesneden
  en niet de tijd verlengd. Fase 3 wacht niet langer.

**Waarom:**
- Het is een wens van de gebruiker en motivatie is de schaarse grondstof (B2). Door B5
  en B22 kost een tweede frontend geen aanpassing aan `sim`.
- raylib heeft dezelfde API voor 2D en 3D, dus de latere 3D-stap is een uitbreiding en
  geen herbouw. Godot en Unity blijven uitgesloten (B3). SDL3 is een alternatief, maar
  voor 3D moet je dan zelf OpenGL doen.
- 2D eerst omdat de echte kost van 3D in assets zit (modellen, textures, animatie), niet
  in code. Dat botst met "geen art-pijplijn" (B4). 2D-tiles uit een CC0-set (bijvoorbeeld
  Kenney) lossen dat op.
- Een tijdsbox omdat grafisch werk geen natuurlijk einde heeft en het huidige spel klein
  is (één kaart, één vijand). Meer graphics dan inhoud is het waarschuwingssignaal.
- Bijkomend voordeel: Windows-native spelen wordt mogelijk (B22-beperking) en een viewer
  is later herbruikbaar om de agent te laten zien (O7).

**Regels:**
1. raylib wordt alleen gelinkt door de grafische executable (`delve_gfx`). Nooit door
   `sim`, `bench`, `tests` of de tekstrenderer.
2. Floats en tijd zijn toegestaan in de weergavelaag (animatie), nooit in `sim` (B14).
   De weergave houdt een eigen view-state bij; `sim::State` blijft de enige waarheid.
3. Pure functies die zonder venster testbaar zijn (bijvoorbeeld tegel → schermpositie)
   krijgen doctest-tests. Tekenen zelf niet.
4. Assets komen in `assets/`, met de licentie van de bron erbij in de repo.
5. Geen asset-pipeline, geen laadframework: een tilesheet en een paar `LoadTexture`-
   aanroepen (projectregel 4).

**Gevolg:** projectplan, README en CLAUDE.md zijn bijgewerkt. Fase 3 begint na G3.

**Uitkomst:** G0–G3 zijn gehaald binnen de tijdsbox, G2 (beurtanimatie) inbegrepen. Zie
B24 voor het vervolg.

---

### B24 — 3D-weergave: nu niet, wel als mogelijke toekomst (beslist bij G3)

**Beslissing:** we blijven bij de 2D-tilefrontend (`delve_gfx`) en de terminalversie.
Een 3D-weergave wordt **niet** gebouwd voor fase 3, maar blijft nadrukkelijk een
mogelijke uitbreiding. De simulatie blijft in elk geval een 2D-integergrid.
Beslist op 29 september 2026, na een geslaagde playtest van fase 2b.

**Waarom nu niet:**
- Fase 3 is de kern van het project en het leerdoel (B1). Meer weergavewerk vóór de
  agent leert, verschuift het zwaartepunt.
- 3D verandert het spel niet: er is geen hoogte in de simulatie. Het spel heeft één kaart
  en één vijand, dus 3D toont weinig wat 2D niet ook toont (het waarschuwingssignaal
  "meer graphics dan spelinhoud" uit B23).
- De echte kost van 3D zit in assets (modellen, textures, animatie), niet in code.
- In WSLg draait OpenGL hier op `llvmpipe` (software, zie `omgeving.md`). Voor 2D geen
  probleem; voor 3D onbekend en nog te meten.
- De 2D-versie voldoet aan de wens uit het begin: een grafische weergave die mooi genoeg is.

**Als 3D later komt — hoe de overgang zou lopen** (een schets, geen belofte):
- *Wanneer het de moeite waard wordt:* wanneer er inhoud is die 3D iets toevoegt, dus
  grotere gegenereerde levels (fase 4), een agent die zichtbaar leert (fase 5–6, O7), of
  simpelweg tijd en zin. Vroegste natuurlijke moment: na fase 3, uiterlijk fase 6.
- *Eerste stap:* een spike van 2–3 uur in een aparte executable (bijvoorbeeld
  `delve_gfx3d`): kubussen voor muren, een camera, licht, en spelers/vijanden als
  eenvoudige vormen. Meten of dat op de beschikbare rendering vloeiend loopt, en
  onderzoeken of WSL GPU-versnelling kan geven (nog niet getest).
- *Wat hergebruikt wordt:* dezelfde `State`, `input`, `level`, en de pure delen van de
  weergave (`anim`: `TurnDelta`, interpolatie, flits). Alleen het tekenen en de camera
  zijn nieuw.
- *Wat niet verandert:* `sim`, de determinismetest en de golden hash, de benchmark, de
  RL-binding. 3D is weergave; agents merken er niets van.
- *Assets:* eerst primitieven met kleur en licht (geen artiest nodig). Modellen pas als
  het de moeite waard blijkt, met een CC0-bron zoals bij B23.
- *Grens:* hoogte of een z-as **in de simulatie** is een aparte, veel zwaardere beslissing.
  Ze verandert `State`, de observatie en de actieruimte (O4, B7) en hoort niet bij dit
  besluit.

**Regels om de weg open te houden (kosten niets):**
1. Pure weergavelogica (positie, animatie, tegelnummers) blijft gescheiden van
   raylib-tekencode, zoals nu in `layout` en `anim`.
2. raylib blijft beperkt tot de grafische executables (B23, regel 1).
3. Nieuwe spelmechanica wordt in `sim` ontworpen zonder aan een weergave te denken; de
   weergave volgt de state en niet omgekeerd.

**Heroverwegen wanneer:** fase 3 werkt en het project een visuele stap kan gebruiken, of
wanneer een van de bovenstaande momenten zich voordoet. Bij een besluit: nieuwe B-nummer,
met de meting van de spike erbij.

---

## Nog open

### ~~O1 — Implementatietaal~~ → beslist, zie B10

Oorspronkelijke afweging, bewaard als context:

**Kandidaten:** Rust, C, C++, Zig.

**Afwegingen**
- *Rust* — grootste leerwinst, moderne toolchain, uitstekende bindings naar Python via
  PyO3. Wel een steile leercurve bovenop een nieuw domein.
- *C* — sluit het beste aan bij RL-tooling die native C-environments ondersteunt;
  eenvoudige FFI. Minder comfortabel, meer voetangels.
- *C++* — bekend terrein voor veel ingenieurs, maar daardoor ook minder leerwinst.
- *Zig* — interessant, maar een klein ecosysteem betekent dat je meer zelf uitzoekt.

**Regel die hier geldt:** verander één of twee dingen tegelijk. Nieuwe taal én nieuw
domein kan. Daar C nog bij nemen omdat de RL-bibliotheek dat prettig vindt, is een
verandering te veel.

**Beslist:** 29 september 2026 → C++20 ([B10](#b10--c20-als-implementatietaal-was-o1)).

---

### O2 — RL-stack

**Kandidaten**
- PufferLib met PyTorch — gebouwd voor snelle gevectoriseerde custom environments,
  ondersteunt environments in C, en hun PPO haalt in de orde van miljoenen steps per
  seconde
- CleanRL — single-file implementaties, minder snel maar veel leerzamer om te lezen
- Een JAX-aanpak in de stijl van Craftax — extreem snel, maar dwingt tot een functionele
  simulatiestijl die botst met "een engine bouwen"
- Eigen PPO — maximale leerwinst, maximale kans om vast te lopen op een subtiele bug

**Te beslissen:** aan het begin van fase 3. Overweging: begin met een bestaande
implementatie zodat een niet-werkende agent aan jóuw environment ligt en niet aan jouw
PPO. Eigen implementatie kan later als fase 6.

---

### ~~O3 — Rendering~~ → beslist, zie B22

---

### O4 — Observatie-representatie voor de agent

Grid-tensor (kanalen per entiteitstype, CNN-policy) versus een compacte feature vector
(MLP-policy). Beïnvloedt trainingssnelheid en generalisatie sterk.

**Te beslissen:** fase 3, waarschijnlijk na experimenteren met beide.

---

### O5 — Reward-ontwerp

Hoeveel shaping is nodig? Kandidaten: beloning voor nieuwe tegels, afstand tot doel,
straf per beurt, intrinsieke exploratiebeloningen. Te veel shaping levert een agent op
die de shaping optimaliseert in plaats van het spel.

**Te beslissen:** empirisch, in fase 3. Begin minimaal en voeg toe zodra blijkt dat er
niets geleerd wordt.

---

### O6 — Publiek of privé

Open source vanaf het begin, een publieke devlog, of alles privé tot er iets staat?

Overweging: een devlog dwingt tot expliciteren en dat is een leerdoel op zich. Maar
publiek werken voegt ook druk toe aan een project dat expliciet geen deadline heeft.

**Te beslissen:** geen haast.

---

### O7 — Inzet als lesmateriaal

Een agent die zichtbaar leert spelen is uitstekend demonstratiemateriaal voor het
vakdidactische werk. Waard om in het achterhoofd te houden bij fase 5 en 6, maar geen
ontwerpcriterium.

**Te beslissen:** niet urgent.
