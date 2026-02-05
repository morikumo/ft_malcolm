## 1) Environnement OBLIGATOIRE

D’après le sujet (chapitre IV ) :

a) Une **VM Linux**
b) Kernel **> 3.14**
c) Testé sur Debian (idéalement)

### Configuration minimale recommandée

* 2 machines virtuelles sur le **même réseau**

  * VM1 : **attaquant** (ft_malcolm)
  * VM2 : **cible**
* Réseau : **Bridge** ou **Host-only**

  * ❌ NAT seul (souvent pas de broadcast ARP visible)

---

## 2) Vérifier les IP et MAC AVANT de lancer ft_malcolm

### Sur la cible (VM2)

```bash
ip a
```

Exemple :

```
eth0: 10.12.10.22
MAC : 10:dd:b1:aa:bb:cc
```

Puis :

```bash
arp -n
```

➡️ Note l’IP **qui sera spoofée** (source IP)

---

### Sur l’attaquant (VM1)

```bash
ip a
```

Exemple :

```
eth0: 10.12.255.255
MAC : ff:bb:ff:ff:ee:ff
```

---

## 3) Lancer ft_malcolm (ordre STRICT des arguments)

Sujet – chapitre V  :

```
./ft_malcolm source_ip source_mac target_ip target_mac
```

### Exemple réel

```bash
sudo ./ft_malcolm \
10.12.255.255 ff:bb:ff:ff:ee:ff \
10.12.10.22 10:dd:b1:aa:bb:cc
```

Tu dois voir :

```
Found available interface: eth0
Waiting for ARP request...
```

👉 À ce stade **rien n’est encore envoyé** (important à dire en défense).

---

## 4) Déclencher une requête ARP depuis la cible

Sur la **VM cible** :

```bash
ping 10.12.255.255
```

Pourquoi ?
Parce que :

* la cible ne connaît pas la MAC associée
* elle envoie une **ARP Request en broadcast**

---

## 5) Ce que ft_malcolm doit afficher

Sur l’attaquant :

```
ARP request detected. Sending spoofed reply...
Sent ARP reply. Exiting.
```

➡️ Le programme :

* a détecté **la bonne requête**
* a envoyé **UNE réponse**
* s’est **terminé proprement**

---

## 6) Vérifier le résultat (preuve finale)

Sur la **cible** :

```bash
arp -n
```

Résultat attendu :

```
10.12.255.255  ether  ff:bb:ff:ff:ee:ff  C  eth0
```

🎯 **Objectif du projet atteint** :

* l’IP spoofée est associée à la fausse MAC
* sans MITM permanent
* sans flood
* sans daemon

---

## 7) Tests d’erreurs (TRÈS IMPORTANT À FAIRE)

Le correcteur va essayer ça.

### IP invalide

```bash
./ft_malcolm 10.11.11.1111 aa:bb:cc:dd:ee:ff ...
```

Sortie attendue :

```
ft_malcolm: unknown host or invalid IP address
```

---

### MAC invalide

```bash
./ft_malcolm ... aaa:bb:cc:dd:ee:ff
```

Sortie attendue :

```
ft_malcolm: invalid mac address
```

---

### Ctrl+C

Pendant l’écoute :

```
Ctrl + C
```


Pour vérifier que l'attaque à fonctionner :
```
ip neigh flush all (cible)

./ft_malcolm (attaquant)

ping (cible)

ip neigh (cible)
```