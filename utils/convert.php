<?php

//to be called from command line from inside utils folder. fetches metadata and writes into quran/suras.txt file to be included as resource file

$xmlFile = file_get_contents('http://tanzil.net/res/text/metadata/quran-data.xml');
$metadata = new SimpleXMLElement($xmlFile);
$suras = $metadata->suras->sura;

$result = "index|ayas|start|name|tname|ename|type\n";

foreach ($suras as $sura) {
    $result .= $sura['index'].'|'.$sura['ayas'].'|'.$sura['start'].'|'.$sura['name'].'|'.$sura['tname'].'|'.$sura['ename'].'|'.$sura['type']."\n";
}

file_put_contents('../quran/suras.txt', $result);
