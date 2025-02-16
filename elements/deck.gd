extends Node2D

const CARD_SCENE = preload("res://elements/card.tscn")

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

func add_card(card: Card):
	ChildExchange.exchange(card, self)
	cards.append(card)
	card.tween_position(Vector2.ZERO, 0, Hand.CARD_MOVEMENT_DURATION )
	get_tree().create_timer(Hand.CARD_MOVEMENT_DURATION / 2.).timeout.connect(func(): card.z_index=len(cards))
