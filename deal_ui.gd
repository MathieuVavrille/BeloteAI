extends Control
const CARD_SCENE = preload("res://card.tscn")


var cards = []

func _ready() -> void:
	for rank in [1, 7, 8, 9, 10, 11, 12, 13]:
		for suit in Card.ALL_SUITS:
			var card = CARD_SCENE.instantiate()
			card.suit = suit
			card.rank = rank
			cards.append(card)
	cards.shuffle()

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass


func _on_new_card_pressed() -> void:
	var dealt_card = cards.pop_back()
	$Bottom/Hand.create_card(dealt_card.suit, dealt_card.rank)
