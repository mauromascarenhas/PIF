# Operações de entrada e saída

## Entrada

Para efetuar a leitura dos dados, você deve utilizar a função `leia` com as respectivas variáveis como argumento (separadas por vírgula). Note que as variáveis devem ser previamente declaradas.

Também é possível perceber que não é necessário preocupar-se com o tipo da variável, pois, a "função" `leia` já faz o trabalho de conversão de entrada sozinha.

Ex.:

```
	nome, sobrenome : literal
	ano_nasc, mes_nasc : numérico
	joga_bola : booleano
	
	leia nome, sobrenome, ano_nasc, mes_nasc, joga_bola
```

**ATENÇÃO! ** cada linguagem de saída lida com a entrada do tipo booleana de um jeito diferente: enquanto em C e C++ os valores fornecidos devem ser `0` e `1` para `falso` e `verdadeiro`, respectivamente, em Java, a entrada deve ser dada por `false` e `true`. O mesmo ocorre para variáveis numéricas que em C e C++ usam o `.` (ponto) como separador decimal, já o java utiliza a localização, que, caso seja pt-BR, será a `,` (vírgula). Note que o código PIF não deverá ser alterado (constantes decimais separadas por ponto e valores verdades dados por `verdadeiro` e `falso`).

## Saída

Para escrever/imprimir os dados na saída padrão da aplicação (normalmente o console), você deverá fazer o uso da função `escreva` com seus respectivos argumentos (**ATENÇÃO! ** diferentemente da "função" de entrada, os argumentos devem ser separados por '+', sem aspas).

Assim como na função de entrada, também é possível perceber que não é necessário preocupar-se com o tipo da variável, pois, a "função" `escreva` já faz o trabalho de conversão de entrada sozinha.

Ex.:

```
	escreva "Ola mundo"
	
	[...]
	
	escreva "Seu nome eh " + nome + "."
	
	[...]
	
	escreva "Seu nome eh " + nome + " " + sobrenome + ". Voce nasceu em : " + mes_nasc + "/" + ano_nasc + ". Joga bola? " + joga_bola
```

## Observações gerais

- Note que todas as variáveis devem estar previamente declaradas, caso contrário, o código sequer compilará!
- Operações lógicas (booleanas) e aritméticas (contas) não são permitidas nas operações de entrada e saída de dados.
- Caracteres especiais em tipos literais (tais como `\n`, `\t` e `\r`) são permitidos.

## Links úteis (aprenda mais)

1. [Índice geral.](../README.md)

# Licença

Todo o conteúdo de documentação encontra-se sob a Licença Creative Commons Attribution-Share Alike 4.0 International (CC-BY-SA).

<p align="center">
	<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/"><img alt="Licença Creative Commons" style="border-width:0" src="https://i.creativecommons.org/l/by-sa/4.0/88x31.png" /></a><br />Este trabalho está licenciado com uma Licença <a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/">Creative Commons - Atribuição-CompartilhaIgual 4.0 Internacional</a>.
</p>
