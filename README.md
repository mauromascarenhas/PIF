# PIF

Repositório de desenvolvimento da linguagem de programação PIF

# Aprendendo a programar...

Esta linguagem é extremamente simples e amigável para programadores iniciantes, contando com diversos exemplos e documentação completa a respeito do uso e das regras.

## Documentação

A documentação da linguagem contém a explicação de como um programa escrito em PIF deve ser escrito:

1. [Início.](./docs/README.md)
1. [Índice geral.](./docs/linguagem/README.md)
1. [Entrada e saída de dados.](./docs/linguagem/operacoes_es.md)
1. [Operações com o tipo literal.](./docs/linguagem/operacoes_literal.md)
1. [Estruturas condicionais.](./docs/linguagem/condicionais.md)
1. [Estruturas de repetição.](./docs/linguagem/repeticao.md)

## Exemplos

1. [Exemplos básicos entrada e saída de dados](./exemplos/1.%20basico_operacoes_es/)
1. [Exemplos de operações com o tipo literal (cadeia de caracteres ou palavras)](./exemplos/2.%20operacoes_literal/)
1. [Exemplos de uso de estruturas condicionais (se...então, senão se ... então, senão ...)](./exemplos/3.%20estruturas_condicionais/)
1. [Exemplos de uso de estruturas de repetição (faça ... enquanto , enquanto ... faça) - básicos e um pouco avançados](./exemplos/4.%20estruturas_repeticao/)

Também há alguns exemplos do que não deve ser feito (alguns programas podem nem chegar a compilar)

1. [Exemplos de código incorreto](./exemplos/0.%20erros/)

# Compilando o código ({programa}.pifc)

Para compilar o código, basta baixar o compilador ou compilá-lo através do código fonte e seguir as instruções conforme indicado na [documentação do compilador](./docs/compiler/README.md).

# Baixando e instalando

## Windows

Há distribuições binárias a partir da página de [*releases*](https://github.com/mauromascarenhas/PIF/releases).
Vá para os detalhes da última versão e baixe o arquivo pif_compiler-X.X.XX_x{64|86}.exe, onde X.X.XX é a versão e x{64|86} é a arquitetura compilada (escolha a arquitetura compatível com a sua máquina). Depois, basta seguir os passos do instalador e executar o compilador através da linha de comando ou usando o [PIF IDE](https://github.com/mauromascarenhas/PIF-IDE).

Alternativamente, você poderá compilar o projeto a partir do código fonte, conforme descrito na sessão **Compilando a partir do código fonte**.

## Linux

Para Linux, você deverá compilar o projeto a partir do código fonte, conforme descrito na sessão **Compilando a partir do código fonte**.

# Compilando a partir do código fonte

O código fonte está sendo testado tanto no Windows (MSVC2017_{x64,x86}, MinGW_{x86,x64}), quanto no Linux (Fedora GCC_x64).

Para compilar o código fonte, primeiramente você deverá seguir alguns passos, dependendo de seu SO.

## Windows

1. (Opcional) Baixe e instale o VS Community 2019 (caso você deseje utilizar próprio compilador C++ do visual studio) - Certifique-se que o kit de desenvolvimento C++ está selecionado.
1. Visite a [página de download da versão de código aberto do Qt](https://www.qt.io/download-qt-installer) (dê preferência ao instalador online).
1. Execute o instalador (talvez você precise fazer um cadastro, mas não se preocupe, não haverá cobrança).
1. Selecione os itens que deseja baixar e instalar (recomendamos que a última versão LTS seja a utilizada).
1. Nas opções "tools" marque também as versões mais recentes do MinGW (é muito útil caso você não queira usar o compilador da MS).
1. Avançe e pronto!
1. Abra o recém instaldo QtCreator, abra o arquivo "compiler.pro" através da opção de abertura de projetos na IDE, selecione os kits válidos e avance.
1. Forneça os parâmetros de linha de comando conforme indicado na [documentação do compilador](./docs/compiler/README.md).
1. Clique <kbd>CTRL</kbd> + <kbd>R</kbd> e o projeto será compilado e executado (de acordo com os argumentos fornecidos).

## Linux

- Instale as dependências conforme descrito em [Qt for Linux/X11](https://doc.qt.io/qt-5/linux.html).
- Agora você tem 2 opções:

1. Instalar o Qt diretamente do site do desenvolvedor.
1. Instalar o Qt através de pacotes de desenvolvimento disponíveis na distro de sua preferência.

### Instalação a partir do site oficial

1. Visite a [página de download da versão de código aberto do Qt](https://www.qt.io/download-qt-installer).
1. Execute o instalador (talvez você precise fazer um cadastro, mas não se preocupe, não haverá cobrança).
1. Selecione os itens que deseja baixar e instalar (recomendamos que a última versão LTS seja a utilizada).
1. Avançe e pronto!
1. Abra o recém instaldo QtCreator, abra o arquivo "compiler.pro" através da opção de abertura de projetos na IDE, selecione os kits válidos e avance.
1. Forneça os parâmetros de linha de comando conforme indicado na [documentação do compilador](./docs/compiler/README.md).
1. Clique <kbd>CTRL</kbd> + <kbd>R</kbd> e o projeto será compilado e executado (de acordo com os argumentos fornecidos).

### Fedora/RHEL/CentOS

1. Digite os seguintes comandos:

```
	$ sudo dnf upgrade
	$ sudo dnf install qt5-devel
	$ cd <PIF_PROJ_PATH/src/>
	$ mkdir build
	$ cd build
	$ qmake ../compiler/compiler.pro
	$ make
```

Pronto! Assim que terminar você poderá rodar o executável conforme a [documentação do compilador](./docs/compiler/README.md).

```
	$ ./pifc [args]
```

- Alternativamente, você também pode usar o Qt creator (mas não se esqueça de adicionar o qmake ao PATH).

### Ubuntu/Debian e derivados...

1. Digite os seguintes comandos:

```
	$ sudo apt-get update && sudo apt-get upgrade
	$ sudo apt-get install qtbase5-dev qt5-qmake
	$ cd <PIF_PROJ_PATH/src/>
	$ mkdir build
	$ cd build
	$ qmake ../compiler/compiler.pro
	$ make
```

Pronto! Assim que terminar você poderá rodar o executável conforme a [documentação do compilador](./docs/compiler/README.md).

```
	$ ./pifc [args]
```

- Alternativamente, você também pode usar o Qt creator (mas não se esqueça de adicionar o qmake ao PATH).

# Licença

Todo o desenvolvimento da linguagem de programação PIFC encontra-se sob a licença de código aberto [LGPL V3](./LICENCE) cuja cópia encontra-se disponível na raíz deste repositório. Já a documentação encontra-se sob a Licença Creative Commons Attribution-Share Alike 4.0 International (CC-BY-SA).

<p align="center">
	<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/"><img alt="Licença Creative Commons" style="border-width:0" src="https://i.creativecommons.org/l/by-sa/4.0/88x31.png" /></a><br />Este trabalho está licenciado com uma Licença <a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/">Creative Commons - Atribuição-CompartilhaIgual 4.0 Internacional</a>.
</p>