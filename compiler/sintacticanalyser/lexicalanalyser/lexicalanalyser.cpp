LexicalAnaliser::LexicalAnaliser() :
	LANGUAGE_TEMPLATE("((numérico|literal|programa|senão|se|fim-se|enquanto|fim-enquanto|fim-programa|leia|escreva)\\b|([:,])|([\\+\\-\\*\\/=])|(<=|>=|<|>|!=)|(\\d+(\\.\\d+)*)|([a-zA-ZÇ-ÜÀ-ÁãÃÊ-ÏÌÓ-Ýá-ñ][\\wÇ-ÜÀ-ÁãÃÊ-ÏÌÓ-Ýá-ó]*)|(\\t))|([^\\s\\1])")
	{
		
}

LexicalAnaliser::~LexicalAnaliser(){
	//FREE STUFF
}