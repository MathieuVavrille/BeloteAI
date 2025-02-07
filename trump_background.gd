extends Node2D

func set_trump(trump):
	var trump_color = null
	match trump:
		Card.Suit.HEARTS: trump_color = "hearts"
		Card.Suit.DIAMONDS: trump_color = "diamonds"
		Card.Suit.CLUBS: trump_color = "clubs"
		Card.Suit.SPADES: trump_color = "spades"
	var texture = load("res://assets/suits/" + trump_color + ".png")
	for child in get_children():
		child.texture = texture
		child.visible = true
