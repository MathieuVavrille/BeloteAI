extends Node2D
class_name CardPicker

const CARD_SCENE = preload("res://elements/card.tscn")

@export var hand: SimpleHand
var cards = []
const CARDS_HEIGHT = 240 / 2
const CARDS_WIDTH = 168 / 2
const CARDS_SPACING = 10

func rank_to_index(rank):
	if rank == 1:
		return 7
	return rank - 7

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	for i in range(8):
		var rank = [7, 8, 9, 10, 11, 12, 13, 1][i]
		for suit in Card.ALL_SUITS:
			var card = CARD_SCENE.instantiate()
			card.suit = suit
			card.rank = rank
			cards.append(card)
			card.position = Vector2((i - 3.5) * (CARDS_WIDTH + CARDS_SPACING), (suit - 1.5) * (CARDS_HEIGHT + CARDS_SPACING) )
			add_child(card)
			card.flip()
			card.mouse_movement.connect(on_card_mouse_movement)
			

func on_card_mouse_movement():
	for card in cards:
		if card.is_hovered:
			card.tween_scale(1.1)
		else:
			card.tween_scale(1.)

func _input(event):
	if event is InputEventMouseButton and event.pressed:
		for i in range(len(cards)):
			if cards[i].is_hovered:
				var selected_card = cards.pop_at(i)
				selected_card.mouse_movement.disconnect(on_card_mouse_movement)
				hand.add_card(selected_card)
				return

func add_card(card):
	ChildExchange.exchange(card, self)
	card.mouse_movement.connect(on_card_mouse_movement)
	cards.append(card)
	var goal_position = Vector2((rank_to_index(card.rank) - 3.5) * (CARDS_WIDTH + CARDS_SPACING), (card.suit - 1.5) * (CARDS_HEIGHT + CARDS_SPACING) )
	card.tween_position(goal_position, 0, Hand.CARD_MOVEMENT_DURATION)
