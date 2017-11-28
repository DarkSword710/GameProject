#include "../../dep/inc/xml/rapidxml.hpp"
#include "../../dep/inc/xml/rapidxml_utils.hpp"
#include "../../dep/inc/XML/rapidxml_iterators.hpp"
#include "../../dep/inc/XML/rapidxml_print.hpp"
#include <map>
#include <string>
#include <iostream>
#include <sstream>


struct EnemyAtributes
{
	int vitalidad;
	int dano;
	int defensa;
	int frecuenciaDeAtaque;
	int experiencia;
};

void main() {
	std::map<std::string, EnemyAtributes> enemigos;

	rapidxml::xml_document<> doc;

	std::ifstream file("../../res/files/enemies.xml");
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();

	std::string content(buffer.str());

	doc.parse<0>(&content[0]);

	rapidxml::xml_node<> *pRoot = doc.first_node();

	EnemyAtributes temp;
	std::string aux;

	for (rapidxml::xml_node<> *pNode = pRoot->first_node("enemigo"); pNode; pNode = pNode->next_sibling()) {

		std::cout << pNode->name() << ':' << ' ' << pNode->first_attribute()->value() << '\n';

		for (rapidxml::xml_node<> *pNodeI = pNode->first_node(); pNodeI; pNodeI = pNodeI->next_sibling()) {
			

			std::cout << pNodeI->name() << ':' << pNodeI->value() << '\n';
			aux = pNodeI->name();
			
			if (aux == "vitalidad")
			{
				temp.vitalidad = atoi(pNodeI->value());
			}
			else if (aux == "dano")
			{
				temp.dano = atoi(pNodeI->value());
			}
			else if (aux == "defensa")
			{
				temp.defensa = atoi(pNodeI->value());
			}
			else if (aux == "frecuenciaAtt")
			{
				temp.frecuenciaDeAtaque = atoi(pNodeI->value());
			}
			else if (aux == "experiencia")
			{
				temp.experiencia = atoi(pNodeI->value());
			}
		}

		enemigos[pNode->first_attribute()->value()] = temp;
		std::cout << '\n';

	}
}
