extends Control
const CARD_SCENE = preload("res://card.tscn")

var hand_to_deal = 0

func _ready():
	for i in range(32):
		get_tree().create_timer(0.5 / 4. * i).timeout.connect(_on_new_card_pressed)

func _on_new_card_pressed() -> void:
	var dealt_card = $Deck/Deck.cards.pop_back()
	[$Bottom/Hand, $Left/Hand, $Top/Hand, $Right/Hand][hand_to_deal%4].add_child_card(dealt_card)  # create_card(dealt_card.suit, dealt_card.rank)
	hand_to_deal += 1
