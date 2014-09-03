package testes;

import java.util.Stack;

public class CaixaDeTestes {

	Stack<Teste> testes;

	int total;
	int concluidos;

	public Teste getTesteAtIndex(int i) {
		return testes.get(i);
	}

	public Stack<Teste> getResultados() {

		return testes;

	}

	public void setTestes(Stack<Teste> testes) {
		this.testes = testes;
	}

	public int getTotal() {
		return total;
	}

	public void setTotal(int total) {
		this.total = total;
	}

	public int getConcluidos() {
		return testes.size();
	}

	public CaixaDeTestes(int numeroDeTestes) {
		total = numeroDeTestes;
		testes = new Stack<Teste>();
		concluidos = 0;
	}

	public void gravarResultadosDeTeste(Teste t) {
		testes.add(t);
	}

	public Teste getUltimoResultadoDeTeste() {
		return testes.lastElement();
	}

}
