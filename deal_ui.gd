extends Control
const CARD_SCENE = preload("res://card.tscn")

var hand_to_deal = 0

func _ready():
	for i in range(32):
		get_tree().create_timer(Hand.CARD_MOVEMENT_DURATION / 4. * i).timeout.connect(deal_card)
	for hand in [$Bottom/Hand, $Left/Hand, $Top/Hand, $Right/Hand]:
		hand.play_card.connect($Deck/Trick.add_child_card)

func deal_card() -> void:
	var dealt_card = $Deck/Deck.cards.pop_back()
	[$Bottom/Hand, $Left/Hand, $Top/Hand, $Right/Hand][hand_to_deal%4].add_child_card(dealt_card)  # create_card(dealt_card.suit, dealt_card.rank)
	hand_to_deal += 1
