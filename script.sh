#!/bin/bash

################################################################################################
#TESTS
# Faire attention a correctement remplir les deux tableaux suivants.
# Garder une logique dans l'ordre des tests.
# Commenter le but des tests + erreurs potentielles

# Definition des tableaux commandes / sorties attendues
commands=(
    "ls"
	"pwd"
	"cat makefile"
	"cat tests/files/file.txt" # test cat *NORMAL* file
	"cat tests/files/big_file.txt" # test cat *BIG* file
	"cat tests/files/empty.txt" # test cat *EMPTY* file
	"cat tests/files/file_without_permission.txt" # test cat chmod 000
	"cd srcs && pwd" # test cd/ET
	"cd srcd && pwd" # !test cd/ET
	"echo Hello, world!" # w/o quote, multiples arg
    "echo 'Hello, world!'" # single quotes, single arg
	"echo \"Hello, world!\"" # double quotes, single arg
	"echo 'Hello, \"world!\"' I love u" # mix args and quotes
    "echo -nnnnn -n" #no '\n', no args
    "echo -n \$PWD" #no return and VE 
    "echo -n coucou le sang" #no return
#	"echo Hello\"World\"" # str"str" bug connu, test de sensibilite du script
	"export a"
	"export hello world"
	"export hello=world && echo \$hello && unset \$hello && echo \$hello"
	"export HELLO=world && echo \$HELLO && unset \$HELLO && echo \$HELLO"
	"export HELLA='warld' && echo \$HELLA && unset \$HELLA && echo \$HELLA"
	"export ="
	"export A B C"
	"export A = b"
	"export A=b C=d E=f"
	"export 1bob=yolo"
	"unset HOME && echo \$HOME"
)

expected_output=(
	"$(ls)"
	"$(pwd)"
	"$(cat makefile)"
	"$(cat tests/files/file.txt)"
	"$(cat tests/files/big_file.txt)"
	"$(cat tests/files/empty.txt)"
	"$(cat tests/files/file_without_permission.txt)"
	"$(cd srcs && pwd)"
	"$(cd srcd && pwd)"
	"$(echo Hello, world!)"
	"$(echo 'Hello, world!')"
	"$(echo \"Hello, world!\")"
	"$(echo 'Hello, "world!"' I love u)"
    "$(echo -n)"
    "$(echo -n \$PWD)"
    "$(echo -n coucou le sang)"
#	"$(echo Hello"World")"
	"$(export a)"
	"$(export hello world)"
	"$(export hello=world && echo \$hello && unset \$hello && echo \$hello)"
	"$(export HELLO=world && echo \$HELLO && unset \$HELLO && echo \$HELLO)"
	"$(export HELLA='warld' && echo \$HELLA && unset \$HELLA && echo \$HELLA)"
	"$(export =)"
	"$(export A B C)"
	"$(export A = b)"
	"$(export A=b C=d E=f)"
	"$(export 1bob=yolo)"
	"$(unset HOME && echo \$HOME)"
)



################################################################################################
#SCRIPT

# Definition des couleurs (codes d'echappement ANSI)
WHITE='\033[0;37m'
CYAN='\033[0;36m'
MAGENTA='\033[0;35m'
BLUE='\033[0;34m'
YELLOW='\033[0;33m'
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # Reinitialisation de la couleur a la normale

# Verification de la longueur des tableaux
if [ "${#commands[@]}" -ne "${#expected_output[@]}" ]; then
    echo "Erreur : Les tableaux 'commands' et 'expected_output' doivent avoir la même longueur."
    exit 1
fi

echo
echo
echo -e "                ${GREEN}TESTS MINISHELL (WIP)${NC}"
echo
echo

error_count=0

# Gestion des arguments en ligne de commande
if [ "$#" -eq 0 ]; then
    # Si aucun argument n'est specifie, execute tous les tests
    for ((i = 0; i < ${#commands[@]}; i++)); do
        cmd="${commands[i]}"
        expected="${expected_output[i]}"
        j=$((i + 1))

        # Execution de la commande et stockage de la sortie dans une variable
        output=$(./minishell <<< "$cmd")

        # Comparaison de la sortie reelle avec la sortie attendue
        if [[ "$output" == *"$expected"* ]]; then
            echo -e "${MAGENTA}Commande n*$j${NC}: '$cmd' - ${GREEN}Test reussi${NC}"
        else
            echo -e "${MAGENTA}Commande n*$j${NC}: '$cmd' - ${RED}Test echoue${NC}"
           # echo "Sortie reelle : '$output'"
           # echo "Sortie attendue : '$expected'"
			error_count=$((error_count + 1))
        fi
        echo
    done

	# Affiche le nombre d'erreurs detectees
	echo
	if [ "$error_count" -eq 0 ]; then
		echo -e "${GREEN}Aucune erreur detectee, congrats!${NC}"
	else
		echo -e "${RED}Nombre d'erreurs detectees : $error_count"
		echo -e "Relancer le script en specifiant le n* de la commande pour plus de details${NC}"
	fi
	echo
elif [ "$#" -eq 1 ]; then
    # Si un argument est specifie, execute le test correspondant
    test_number="$1"
    if [ "$test_number" -ge 1 ] && [ "$test_number" -le "${#commands[@]}" ]; then
        i=$((test_number - 1))
        cmd="${commands[i]}"
        expected="${expected_output[i]}"
        j=$((i + 1))

        # Execution de la commande et stockage de la sortie dans une variable
        output=$(./minishell <<< "$cmd")

        # Comparaison de la sortie reelle avec la sortie attendue
		echo
        if [[ "$output" == *"$expected"* ]]; then
            echo -e "${MAGENTA}Commande n*$j${NC}: '$cmd' - ${GREEN}Test reussi${NC}"
            echo
			echo -e "${GREEN}Sortie reelle${NC} : '$output'"
			echo
            echo -e "${GREEN}Sortie attendue${NC} : '$expected'"
        else
            echo -e "${MAGENTA}Commande n*$j${NC}: '$cmd' - ${RED}Test echoue${NC}"
            echo
			echo -e "${RED}Sortie reelle${NC} : '$output'"
			echo
            echo -e "${GREEN}Sortie attendue${NC} : '$expected'"
        fi
    else
        echo "Numero de test invalide. Specifier un numero de test entre 1 et ${#commands[@]}."
    fi
else
    echo "Utilisation : $0 [numero-du-test]"
    echo "Si aucun argument n'est specifie, tous les tests seront executes. Sinon, seul le test specifie sera execute."
fi
