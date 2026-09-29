# Beslissingslogboek

Elke beslissing met de reden erbij, zodat later-jij weet waarom iets zo is en niet
opnieuw dezelfde discussie voert. Beslissingen mogen omgegooid worden — noteer dan
waarom.

Laatst bijgewerkt: 29 september 2026

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
- Een vijand naast de speler slaat (schade 1, speler-HP 10). Bewegen van de vijand
  komt in M3.
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

### O3 — Rendering

**Kandidaten:** terminal/ASCII, raylib, SDL, of rechtstreeks een GPU-API zoals wgpu.

**Afweging:** terminal is het snelst tot een speelbare build en volstaat volledig voor
fase 2. Een echte renderer is leerzamer maar kan ook fase 6 zijn. Beslissing hangt af
van of rendering een leerdoel is of een middel.

**Te beslissen:** begin fase 2.

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
