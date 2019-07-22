// Programa Soma gerado automaticamente por PIFC

import java.util.Scanner;

public class Soma {

	public static void main(String[] args) {
		Scanner scanner = new Scanner(System.in);
		double a, b, soma;
		System.out.println("Digite dois numeros:");
		a = scanner.nextDouble();
		b = scanner.nextDouble();
		soma = a + b;
		System.out.println("A soma deles eh : " + Double.toString(soma));
	}

}
