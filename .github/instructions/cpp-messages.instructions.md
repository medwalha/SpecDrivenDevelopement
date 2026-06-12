---
applyTo: "**/*.{hpp,h,cpp,cc,cxx}"
description: Conventions de codage pour les structures de messages C++ générées à partir d'une spec.
---

# Conventions C++ — Structures de messages (style maison)

Ces règles s'appliquent à **tout code C++** généré dans ce projet, en particulier aux structures de messages issues d'une spec PDF / ICD.

> ⚠️ Ce projet utilise un **style maison spécifique** (préfixe `m_`, typedefs courts, classe wrapper sur `reinterpret_cast`, macros `DUMP_FIELD`). **Ne pas substituer** par les conventions standard `std::uint8_t` / `snake_case` — la cohérence avec la base existante prime.

## 1. Typedefs autorisés (et obligatoires)

Tous les champs de message **doivent** utiliser ces typedefs (déclarés dans `include/messages/Common.hpp`) :

| Typedef         | Équivalent           | Usage                                  |
|-----------------|----------------------|----------------------------------------|
| `uint8`         | `std::uint8_t`       | Octet non signé                        |
| `int8`          | `std::int8_t`        | Octet signé                            |
| `uint16`        | `std::uint16_t`      | Mot 16 bits non signé                  |
| `int16`         | `std::int16_t`       | Mot 16 bits signé                      |
| `uint32`        | `std::uint32_t`      | Mot 32 bits non signé                  |
| `int32`         | `std::int32_t`       | Mot 32 bits signé                      |
| `uint64`        | `std::uint64_t`      | Mot 64 bits non signé                  |
| `int64`         | `std::int64_t`       | Mot 64 bits signé                      |
| `Decimal`       | (struct du domaine)  | Prix / quantité décimale               |
| `TimestampNano` | (struct du domaine)  | Horodatage nanoseconde                 |
| `char[N]`       | tableau C            | Chaîne de longueur fixe (ASCII)        |

❌ **Interdits** : `std::uint8_t`, `unsigned int`, `int`, `long`, `short`, `std::array<char,N>` pour les chaînes ASCII fixes.

## 2. Layout binaire & packing

- **Toute** struct de message est encadrée par `#pragma pack(push, 1)` / `#pragma pack(pop)`.
- La struct s'appelle `<NomMessage>Struct` (ex: `PostTradeStruct`).
- Layout = mapping binaire **1:1** avec le PDF, dans l'ordre exact des champs.
- Pas de `std::string`, `std::vector`, pointeur ou type non trivialement copiable dans la struct.
- Pour une chaîne ASCII de longueur N : `char m_FooBar[N];` (pas de `std::array`).

## 3. Nommage

| Élément                | Convention                  | Exemple                  |
|------------------------|-----------------------------|--------------------------|
| Struct binaire         | `<Name>Struct` PascalCase   | `PostTradeStruct`        |
| Classe wrapper         | `<Name>` PascalCase         | `PostTrade`              |
| Champ de struct        | `m_<PascalCase>`            | `m_MsgSeqNum`            |
| Getter                 | `get<PascalCase>()`         | `getMsgSeqNum()`         |
| Membre de classe       | `m_<PascalCase>`            | `m_MessageHeader`        |
| Constante de message   | `TYPE`, `NAME`, `SIZE`      | `static constexpr ...`   |
| Constante d'opcode     | `MSG_ID_<UPPER_SNAKE>`      | `MSG_ID_POST_TRADE`      |
| Enum class             | PascalCase                  | `MDOriginType`           |
| Namespace              | `lower::snake`              | `protocol::messages`     |

## 4. Anatomie d'un fichier message

Chaque message vit dans `include/messages/<Name>.hpp` et contient **dans l'ordre** :

1. En-tête de traçabilité (SPDX, PDF source, section, version, date).
2. Includes (`Common.hpp`, `MessageHeader.hpp`, types réutilisables).
3. `#pragma pack(push, 1)` + `struct <Name>Struct { ... };` + `#pragma pack(pop)`.
4. Classe wrapper `<Name>` :
   - membres `m_MessageHeader` (const) et `m_Struct` (référence const sur la zone binaire),
   - constantes `TYPE`, `NAME`, `SIZE`,
   - constructeur `explicit <Name>(InputData const & data)`,
   - getters un par champ (voir §5),
   - `virtual void dump(std::ostream & os) const`.
5. `inline std::ostream& operator<<(std::ostream&, <Name> const &)` qui appelle `dump()`.

### Squelette canonique

```cpp
#pragma pack(push, 1)
struct PostTradeStruct
{
    uint8   m_MDUpdateAction;
    int64   m_MsgSeqNum;
    char    m_SecurityID[12];
    Decimal m_MDEntryPx;
    // ... ordre exact de la spec ...
};
#pragma pack(pop)

class PostTrade
{
    MessageHeader     const   m_MessageHeader;
    PostTradeStruct   const & m_Struct;
public:
    static constexpr const uint16  TYPE = 201;
    static constexpr const char*   NAME = "PostTrade";
    static const unsigned int      SIZE = MessageHeader::SIZE + sizeof(PostTradeStruct);

    explicit PostTrade(InputData const & data)
    : m_MessageHeader(data)
    , m_Struct(*reinterpret_cast<const PostTradeStruct*>(data.data() + MessageHeader::SIZE))
    {
    }

    // Getters : voir §5
    // dump()  : voir §6
};

inline std::ostream& operator<<(std::ostream& out, const PostTrade& obj)
{
    obj.dump(out);
    return out;
}
```

## 5. Règles des getters

Un getter par champ, dans l'**ordre de la struct**, en **une ligne** :

| Type du champ        | Type retourné       | Forme                                                          |
|----------------------|---------------------|-----------------------------------------------------------------|
| `uint8` / `int8`     | identique           | `uint8 getX() const { return m_Struct.m_X; }`                  |
| `uint16/32/64`, etc. | identique           | `int64 getX() const { return m_Struct.m_X; }`                  |
| `char m_X[N]`        | `std::string`       | `std::string getX() const { return std::string(m_Struct.m_X, N); }` |
| `char m_X` (1 octet) | `char`              | `char getX() const { return m_Struct.m_X; }`                   |
| `Decimal`            | `Decimal`           | `Decimal getX() const { return m_Struct.m_X; }`                |
| `TimestampNano`      | `TimestampNano`     | `TimestampNano getX() const { return m_Struct.m_X; }`          |
| `enum class E`       | `E`                 | `E getX() const { return m_Struct.m_X; }`                      |

## 6. Règles de `dump()`

`dump()` commence **toujours** par `m_MessageHeader.dump(os);`, puis liste les champs via macros :

- `DUMP_FIELD(<Name>)` — affichage standard (utilise `operator<<`).
- `DUMP_FIELD_AS_INT(<Name>)` — pour les champs `uint8` / `int8` afin d'éviter l'affichage en caractère ASCII.

```cpp
virtual void dump(std::ostream & os) const
{
    m_MessageHeader.dump(os);
    os
    DUMP_FIELD_AS_INT(MDUpdateAction)
    DUMP_FIELD(MsgSeqNum)
    DUMP_FIELD(SecurityID)
    // ... ordre exact des champs ...
    ;
}
```

Règles :
- `uint8` / `int8` → **toujours** `DUMP_FIELD_AS_INT`.
- `char[N]` (chaîne) → `DUMP_FIELD`.
- `char` (1 octet visible) → `DUMP_FIELD`.
- Tous les autres types → `DUMP_FIELD`.

Les macros `DUMP_FIELD` et `DUMP_FIELD_AS_INT` sont définies dans `include/messages/Common.hpp`.

## 7. Constructeur — accès au binaire

- `m_MessageHeader(data)` : construit l'en-tête depuis le buffer brut.
- `m_Struct(*reinterpret_cast<const <Name>Struct*>(data.data() + MessageHeader::SIZE))` : aliasage binaire **strict** sur le payload.
- **Aucune copie** de la zone payload.
- L'objet `data: InputData const &` doit rester vivant pendant toute la durée de vie de la classe wrapper. Documenter ce contrat.

## 8. MessageHeader & PacketHeader

- Le **format exact** de `MessageHeader` et `PacketHeader` est **spécifique à chaque protocole**.
- Avant toute génération de message, **demande à l'utilisateur** :
  1. Le contenu / disposition du `MessageHeader` (champs, taille, méthode `dump()`).
  2. Le contenu / disposition du `PacketHeader` (s'il existe) et sa relation au `MessageHeader`.
- Si non fourni, génère un **stub** `include/messages/MessageHeader.hpp` avec un `// TODO:` clair et signale-le.
- Ne jamais inventer des champs d'en-tête sans confirmation.

## 9. Constantes de message

Dans la classe wrapper :

```cpp
static constexpr const uint16  TYPE = <opcode>;            // ID du message dans la spec
static constexpr const char*   NAME = "<Name>";            // nom littéral
static const unsigned int      SIZE = MessageHeader::SIZE + sizeof(<Name>Struct);
```

L'opcode doit aussi figurer dans `include/messages/MessageIds.hpp` sous la forme :
```cpp
inline constexpr uint16 MSG_ID_<UPPER_SNAKE> = <opcode>;
```

## 10. Validation à la compilation

Pour **chaque** struct de message, ajouter après la définition :

```cpp
static_assert(sizeof(<Name>Struct) == EXPECTED_PAYLOAD_SIZE,
              "<Name>Struct size mismatch with spec");
static_assert(std::is_standard_layout<<Name>Struct>::value, "must be standard-layout");
static_assert(std::is_trivially_copyable<<Name>Struct>::value, "must be trivially copyable");
```

`EXPECTED_PAYLOAD_SIZE` = taille payload **hors** `MessageHeader`, telle que définie par la spec.

## 11. Champs réservés / padding

- Préfixer par `m_Reserved_<n>` ou `m_Pad_<n>`.
- Initialisation à zéro non requise (struct vue en `reinterpret_cast`, pas instanciée).

## 12. Bitfields

- Utiliser un type sous-jacent explicite (`uint8` / `uint16`) :
  ```cpp
  struct StatusFlagsStruct {
      uint8 m_Ready    : 1;
      uint8 m_Error    : 1;
      uint8 m_Mode     : 2;
      uint8 m_Reserved : 4;
  };
  static_assert(sizeof(StatusFlagsStruct) == 1);
  ```
- L'ordre des bits est implementation-defined. Documenter le compilateur cible.

## 13. Interdits

- ❌ `using namespace std;`
- ❌ `std::uint*_t` dans les structs de message (utiliser les typedefs `uint8`/`int64`/...).
- ❌ `std::string`, `std::vector` dans une struct binaire.
- ❌ Macros pour définir des messages.
- ❌ Pointeurs bruts dans une struct de message.
- ❌ Héritage entre messages (composition obligatoire).
- ❌ `int`, `long`, `short`, `unsigned` non qualifiés.
- ❌ Copie du payload dans la classe wrapper (toujours `reinterpret_cast` + `const &`).

## 14. Traçabilité

Chaque header doit contenir en commentaire :
- Nom du PDF source + section / table d'origine.
- Numéro de version de la spec.
- Date de génération.
