# Estruturas de repetição

Esta linguagem suporta duas estruturas de repetição diferentes, a `faça ... enquanto` e a `enquanto`. Em ambos as estruturas a ideia é que o bloco de código que estiver dentro delas seja executado enquanto determinada condição for satisfeita (verdadeira).

A etrutura de repetição `enquanto` é descrita da seguinte forma :

```
	enquanto <condição> faça
		# código a ser executado enquanto a condição for satisfeita
		[...]
	fim-enquanto
```

Já a estrutura `faça ... enquanto` é descrita da seguinte forma :

```
	faça
		# código a ser executado enquanto a condição for satisfeita
		# ou na primeira chamada ao trecho do código
		[...]
	enquanto <condição>
```

Note que o código contido na segunda estrutura será executado ao menos uma vez (é muito útil para realizar a validação de dados de entrada).

Também é importante perceber que a `<condição>` nada mais é do que uma expressão booleana sem assinalamento (não pode haver o operador `=`) e sem operações aritméticas adjacentes, ou seja, operações de soma, subtração, multiplicação e divisão seguidas de comparação. A seguir um exemplo do que pode ser feito e o que não deve ser feito:

### Incorreto

```
	enquanto (x * x) > k faça
		# código a ser executado enquanto a condição for satisfeita
		# correto
		[...]
	fim-enquanto
```

### Correto

```
	quadrado : numérico
	
	quadrado = x * x
	
	enquanto quadrado > k faça
		# código a ser executado enquanto a condição for satisfeita
		[...]
		
		quadrado = x * x
	fim-enquanto
```

## Links úteis (aprenda mais)

1. [Índice geral.](../README.md)

# Licença

Todo o conteúdo de documentação encontra-se sob a Licença Creative Commons Attribution-Share Alike 4.0 International (CC-BY-SA).

<p align="center">
	<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/"><img alt="Licença Creative Commons" style="border-width:0" src="https://i.creativecommons.org/l/by-sa/4.0/88x31.png" /></a><br />Este trabalho está licenciado com uma Licença <a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/">Creative Commons - Atribuição-CompartilhaIgual 4.0 Internacional</a>.
</p>
