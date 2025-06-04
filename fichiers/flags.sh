#!/bin/bash

INPUT_FILE="/data/medailles.csv"
OUTPUT_DIR="/data/flags"

mkdir -p "$OUTPUT_DIR/20px"
mkdir -p "$OUTPUT_DIR/80x60"

# Lire les codes ISO Alpha-2
countries=$(cut -d ',' -f 2 "$INPUT_FILE" | sed '1d')

for country in $countries; do
    iso=$(echo "$country" | tr '[:upper:]' '[:lower:]')
    wget -q -O "$OUTPUT_DIR/20px/${iso}.png" "https://flagcdn.com/w20/${iso}.png"
    wget -q -O "$OUTPUT_DIR/80x60/${iso}.png" "https://flagcdn.com/w80/${iso}.png"
done

echo "Drapeaux téléchargés dans $OUTPUT_DIR."
