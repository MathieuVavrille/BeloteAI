extends Node2D

const CARD_SCENE = preload("res://card.tscn")

var cards = []

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	$FakeCard.visible = false
	for rank in [1, 7, 8, 9, 10, 11, 12, 13]:
		for suit in Card.ALL_SUITS:
			var card = CARD_SCENE.instantiate()
			card.suit = suit
			card.rank = rank
			cards.append(card)
			add_child(card)
	cards.shuffle()
	for i in range(len(cards)):
		cards[i].z_index = i



# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass
