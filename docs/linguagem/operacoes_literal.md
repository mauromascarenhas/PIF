# Operações com variáveis do tipo literal

Apenas as operações de concatenação e assinalamento são permitidas para variáveis de tipos literais (por hora, as operações booleanas (comparação) não são).

Para realizar as operações de concatenação basta realizar o uso do operador '+', sem aspas, entre as variáveis. Note que é possível assinalar/concatenar variáveis de qualquer tipo a um literal.

Ex.:

```
	nome, sobrenome, nome_completo : literal
	
	[...]
	
	nome_completo = nome + " " + sobrenome
```

Ex.:

```
	x : numerico
	res : literal
	
	[...]
	
	res = "O resultado de x eh : " + x
	escreva res
```

É importante notar que não pode haver qualquer outro símbolo (como os de precedência `(` e `)`, por exemplo), pois, operações aritméticas não são permitidas no momento da assinalação do valor de um literal. A seguir, há um pequeno exemplo demonstrando a forma correta e a forma incorreta de se concatenar resultados de operações a literais em PIF (esta mesma afirmação vale para operações booleanas!):

### Incorreto

```
	x, y : numerico
	res : literal
	
	[...]
	
	# INCORRETO! dará erro de compilação
	res = "X + Y = " + ( x + y )
```

```
	x, y : numerico
	res : literal
	
	[...]
	
	# INCORRETO! causará erro semântico (o compilador exibirá os valores de x e y concatenados)
	# Ex.: Suponha x = 20 e y = 30, o res assumiria o seguinte valor:
	# res = "X + Y = 2030"
	
	res = "X + Y = " + x + y 
```

### Correto

```
	x, y : numerico
	res : literal
	
	[...]
	
	soma = x + y
	
	# CORRETO!
	res = "X + Y = " + soma
```

## Links úteis (aprenda mais)

1. [Índice geral.](../README.md)

# Licença

Todo o conteúdo de documentação encontra-se sob a Licença Creative Commons Attribution-Share Alike 4.0 International (CC-BY-SA).

<p align="center">
	<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/"><img alt="Licença Creative Commons" style="border-width:0" src="https://i.creativecommons.org/l/by-sa/4.0/88x31.png" /></a><br />Este trabalho está licenciado com uma Licença <a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/">Creative Commons - Atribuição-CompartilhaIgual 4.0 Internacional</a>.
</p>
