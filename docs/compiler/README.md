# Compilador

## Como usar

### Configurando as variáveis de ambiente

Para utilizar o compilador, você deve adicionar o diretório onde o arquivo "pifc.exe" (ou "pifc" em sistemas unix) se encotra. Caso este passo não seja realizado, você deverá executar todos os comandos diretamente do diretório onde o executável está localizado.

### Execução 

Para executar o compilador, basta utilizar o seguinte comando:

```
	$ pifc [configurações: opcional] "caminho_para_o_código_fonte" [opções : opcional]
```

#### Parâmetros

##### Configurações

Os parâmetros de configuração são opcionais, sendo eles:

- -c, -C: Considera os avisos como erros, abortando a operação de compilação;
- -l ["c", "cpp" ou "java"] : estabelece a saída do arquivo fonte (padrão: "c").

##### Código fonte

O caminho para o código fonte deve ser fornecido **OBRIGATORIAMENTE**. Caso o caminho seja inválido, o compilador abortará a operação.

##### Opções

Os parâmetros de opções são opcionais. Porém, diferentemente das configurações, eles estabelecem os parâmetros de saída da aplicação.

- -o, -O "caminho_para_o_arquivo_de_saida" : Note que, caso esta opção seja utilizada, é **OBRIGATÓRIO** fornecer o caminho para o arquivo de saída ("código compilado"). Note que a extensão do arquivo deve estar de acordo com a saída do arquivo fonte especificado no parâmetro de configuração.

## Código fonte

O código fonte é aberto e está sob a licença de código aberto [LGPL v3](https://github.com/mauromascarenhas/PIF/blob/master/LICENCE). Sendo assim, você poderá adaptá-lo às suas necessidades, desde que o código fonte esteja sob a mesma licença.

