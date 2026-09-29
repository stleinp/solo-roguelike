# Achtergrond: hoe dit project tot stand kwam

Samenvatting van de brainstorm van 29 september 2026. Doel van dit document: later-jij
hoeft niet opnieuw dezelfde afwegingen te maken, en de verworpen ideeën gaan niet
verloren.

---

## 1. Het uitgangspunt

De aanleiding: een educatieve master is inhoudelijk iets heel anders dan vier jaar
industrieel ingenieur informatica, en er ontstond het gevoel komend jaar te weinig met
informatica bezig te zijn. Idee: een ambitieus persoonlijk project ernaast, dat
eventueel ooit geld zou kunnen opbrengen.

De eerste richting was een game, met alles nog open: concept, platform, verdienmodel.

## 2. De verschuiving die alles veranderde

Halverwege het gesprek bleek het geld-aspect niet het werkelijke motief. De kern is:
**bijleren en skills onderhouden**, en het project moet vooral uitdagend en persoonlijk
leuk zijn.

Dat maakte de keuze eenvoudiger in plaats van moeilijker. Marktcijfers, monetisatie en
doelgroep zijn daarmee achtergrondinformatie geworden, geen selectiecriteria. Wat
hieronder staat over markten is bewaard omdat het nuttig is als de vraag ooit terugkomt
— niet omdat het dit project nog stuurt.

---

## 3. Overwogen richtingen en waarom ze afvielen

### 3.1 Indie game op Steam als commercieel product

De cijfers voor 2026 zijn hard. In 2025 verschenen ongeveer 20.000 games op Steam en de
mediane game verdiende daar zo'n $249 bruto; na de platformafdracht blijft daar ongeveer
$174 van over. Voor de eerste maanden van 2026 lag die mediaan rond $290, en haal je de
top 50 titels weg, dan zakt het gemiddelde van ongeveer $120.000 naar ongeveer $1.500.
Mildere schattingen komen op $5.000–$15.000 levenslang, maar het patroon is overal een
extreme power law: een game is statistisch ofwel een doorbraak ofwel nul.

Daarbij: programmeren is misschien 30% van het werk. Art, audio, game feel en vooral
marketing zijn de rest, en daar stranden solo-devs.

**Afgevallen als verdienmodel. Niet afgevallen als leerproject** — daar is het juist
uitstekend.

### 3.2 Mobiele game

Onderzocht als mogelijk makkelijker alternatief. Bouwen is inderdaad lichter,
verdienen veel zwaarder.

Het beeld voor 2026: downloads zijn vlak, schermtijd is verzadigd rond 3,6 uur per
gebruiker per dag, acquisitiekosten stijgen. Retentie is ingestort — D1 rond 26%, D7
rond 10%, D30 onder 4%, en driekwart van de games haalt geen 3% op D28. De industrie
gaf in 2025 $25 miljard uit aan user acquisition. Mobiele ontwikkelaars noemen
concurrentie (33%) en UA-kosten (30%) als hun grootste problemen, en een veelgehoorde
analyse is dat marketing inmiddels belangrijker is dan het product zelf. Het klassieke
hypercasual-model brak toen CPI sneller steeg dan eCPM; wat nu wint is hybrid-casual met
meta-progressie, IAP-economie en live-ops.

**Kernconclusie:** op mobiel koop je distributie, op Steam kun je die nog verdienen.
Wishlists, Next Fest, het aanbevelingsalgoritme en streamers zijn mechanismen die een
solo-dev zonder advertentiebudget kan bespelen. In de app stores bestaat dat equivalent
niet.

**Afgevallen.**

### 3.3 Micro-SaaS, met edtech Vlaanderen als sterkste kandidaat

Dit was de commercieel meest kansrijke richting, en blijft dat.

Het argument: een jaar lang van binnenuit in het Vlaamse secundair onderwijs staan geeft
toegang tot een domein dat vrijwel geen solo-dev heeft. De pijn is goed gedocumenteerd —
83% van de onderwijsprofessionals ervaart veel tot heel veel planlast, en UGent@Work
schatte dat leerkrachten in het secundair ongeveer een derde van hun werktijd als
planlast beschouwen, met een duidelijke link naar uitputting en uitstroom. Smartschool
wordt in datzelfde onderzoek genoemd als iets dat planlast eerder vergroot.

Belangrijke nuance die toen naar boven kwam: 80% wijst naar de Onderwijsinspectie en 78%
naar de koepels als bron van planlast. Je kunt de verplichting dus niet wegnemen, alleen
goedkoper maken. Een tool die verplichte verantwoording van twee uur naar tien minuten
brengt verkoopt; een tool die zegt "laat die administratie gewoon" niet.

**Ideeën die de moeite waard blijven** (bewaard voor later, niet weggegooid):

| Idee | Wie betaalt | Grootste risico |
|---|---|---|
| Autograding en feedback op leerlingencode (informatica SO) | leerkracht/vakgroep | kleine Vlaamse markt, maar code is taalonafhankelijk dus internationaliseerbaar |
| Toets- en oefeningengenerator gekoppeld aan leerplandoelen (KOV, GO!) | leerkracht, ~€6/mnd | concurrentie; verdediging zit in de leerplanmapping |
| Beheer van lessenreeksen over meerdere jaren | leerkracht | gratis alternatief is een map met Word-bestanden |
| Labo- en practicumbeheer techniek | vakgroep/school | trage schoolverkoop |
| Stage-opvolging (TSO/BSO of lerarenopleiding) | school/hogeschool | traagste verkoop, hoogste pijn |

De sterkste van die lijst was de **autograder**: eigen doelgroep, technisch interessant
(sandboxing, statische analyse, testgeneratie, zinvolle feedback), en niet opgesloten in
Vlaanderen.

Realiteitscheck op de opbrengst: individuele leerkrachten betalen weinig. De grootste
speler in die markt zit rond $8,33 per maand. Met 200 betalende leerkrachten kom je op
€1.300–1.600 per maand. Geen bedrijf, wel structureel beter dan de mediane Steam-game.

**Afgevallen voor nu, omdat het doel veranderde.** Als de motivatie ooit verschuift naar
"iets dat geld opbrengt", begint het opnieuw hier — en de stagegesprekken van dit jaar
zijn dan de beste grondstof. Suggestie die toen gedaan werd en nog steeds geldt: vraag
tijdens stage aan tien leerkrachten waar hun tijd naartoe gaat die niks met lesgeven te
maken heeft, en wat ze nu in Excel doen omdat er geen goede tool voor is. Iemand die een
zelfgemaakte spreadsheet trots toont, heeft een gevalideerd probleem.

---

## 4. Hoe het huidige idee ontstond

Nadat het doel verschoof naar leren, kwam de vraag op tafel: welke domeinen hebben de
hoogste leer-dichtheid? Overwogen werd onder meer een emulator (CHIP-8 → Game Boy), een
eigen programmeertaal of interpreter, een game vanaf de systeemkant met eigen ECS en
renderer, procedurele generatie en shaders, iets gedistribueerds van nul, en embedded
werk met microcontrollers.

Uit die lijst kwam de combinatie die dit project werd: **een roguelike met een eigen
engine, plus RL-agents die hem leren spelen.** Het oorspronkelijke voorstel bevatte ook
via RL getrainde vijanden voor de moeilijkheidsgraad; dat is omgezet naar de agent als
meetinstrument — zie [beslissing B8](beslissingen.md#b8--agents-zijn-meetinstrument-geen-tegenstander).

Waarom deze combinatie overtuigde:

- Engine en RL zijn geen losse projecten. Determinisme, headless draaien en hoge
  throughput zijn RL-eisen die precies verklaren waarom je de engine zelf schrijft.
- Het combineert iets nieuws (systeemprogrammeren, engine-architectuur) met iets waar al
  ervaring in zit (machine learning), zodat dat laatste onderhouden wordt.
- De game levert de motivatielaag, de systemen eronder de diepte.
- Het resultaat is uitstekend lesmateriaal. Een agent die zichtbaar leert spelen maakt
  indruk op zestienjarigen, en je staat straks voor de klas als iemand die dit doet, niet
  alleen uitlegt.

---

## 5. Bronnen uit de brainstorm

Marktcijfers indie en mobiel:

- [Indie game revenue data (Ziva)](https://ziva.sh/blogs/indie-game-revenue)
- [Steam Q1 2026 analyse (Games-Stats)](https://games-stats.com/blog/Steam_in_Q12026/)
- [Indie game statistieken 2026 (ShaneTheGamer)](https://www.shanethegamer.com/research/indie-games-statistics/)
- [State of Mobile 2026 (Deconstructor of Fun)](https://www.deconstructoroffun.com/blog/2026/2/2/state-of-mobile-2026)
- [UA- en retentiepijnpunten 2026 (Metricus)](https://metricusapp.com/blog/mobile-game-user-acquisition-distribution-painpoints-2025-2026/)
- [Hybrid casual strategie 2026 (Game Growth Advisor)](https://gamegrowthadvisor.com/blog/2026-04-16-hybrid-casual-game-design-strategy-2026/)
- [Mobiele ontwikkelkosten en Unity 2026-rapport (Galaxy4Games)](https://galaxy4games.com/en/knowledgebase/blog/how-much-does-it-cost-to-develop-a-mobile-game-in-2026)

Onderwijs en edtech:

- [Planlastrapport Onderwijsinspectie (VRT NWS)](https://www.vrt.be/vrtnws/nl/2024/03/20/ruim-vier-op-vijf-leerkrachten-klagen-over-onnodige-administrati/)
- [UGent@Work over planlast en Smartschool](https://www.vlaamsbelang.org/nieuws/verlaag-de-planlast-het-onderwijs)
- [EdTech-tools en prijzen 2026 (Foroes)](https://www.foroes.net/best-edtech-tools-teachers-2026/)

Micro-SaaS als achtergrond:

- [Micro-SaaS marktcijfers (Lovable)](https://lovable.dev/guides/micro-saas-ideas-for-solopreneurs-2026)
- [Micro-SaaS benchmarks (Superframeworks)](https://superframeworks.com/articles/best-micro-saas-ideas-solopreneurs)

Technische referenties voor het project zelf staan in [`referenties.md`](referenties.md).
