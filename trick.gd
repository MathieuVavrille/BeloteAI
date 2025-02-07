extends Node2D

signal trick_won(winner: int)

const RADIUS = 75
const THROW_RANDOMNESS = 20

var start_direction = 0
var cards = []
var best_card_id = null
var trump = Card.Suit.SPADES
var hands = []

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	rotation = start_direction * PI/2
	trump = hands[0].trump
	for hand in hands:
		hand.play_card.connect(add_card)
	var available_cards = []
	for i in range(len(hands[start_direction].cards)):
		available_cards.append(true)
	hands[start_direction].activate_cards(available_cards)


func _process(_delta: float) -> void:
	$Placeholder.position = Vector2(cos($Placeholder.rotation + PI/2), sin($Placeholder.rotation + PI/2)) * RADIUS

const MOVE_SPEED = 0.25
func move_placeholder():
	var rot_tween = create_tween()
	rot_tween.tween_property($Placeholder, "rotation", (len(cards) + start_direction)*PI/2, MOVE_SPEED)


func add_card(card):
	ChildExchange.exchange(card, self)
	cards.append(card)
	if best_card_id == null or compare_cards(cards[best_card_id], card, cards[0].suit):
		best_card_id = len(cards) - 1
	cards.back().tween_position($Placeholder.position, fmod($Placeholder.rotation, 2 * PI) + deg_to_rad(randi()%THROW_RANDOMNESS - THROW_RANDOMNESS / 2.), .5)
	cards.back().z_index = len(cards) - 1
	if len(cards) < 4:
		move_placeholder()
		var opponent_wins = len(cards) - 2 == best_card_id
		hands[(len(cards) + start_direction)%4].activate_possible_cards(cards[0].suit, opponent_wins, null if cards[best_card_id].suit != trump else Card.TRUMP_VALUES[cards[best_card_id].rank])
	else:
		$Placeholder.visible = false
		cards[best_card_id].highlighted = true
		cards[best_card_id].z_index = 4
		get_tree().create_timer(1.).timeout.connect(bring_all_cards)
		get_tree().create_timer(2.5).timeout.connect(flip)
		get_tree().create_timer(2.5 + Card.FLIP_DURATION).timeout.connect(func(): trick_won.emit((start_direction + best_card_id) % 4))

func bring_all_cards():
	for card in cards:
		card.tween_position(Vector2.ZERO, 0, 1.)
func flip():
	for card in cards:
		card.flip()

func set_modulo_rotation(goal_rotation):
	while rotation < goal_rotation:
		rotation += 2 * PI
	while rotation > goal_rotation:
		rotation -= 2 * PI
	if goal_rotation - rotation > rotation + 2 * PI - goal_rotation:
		rotation += 2 * PI

func tween_position(goal_position, goal_rotation, duration):
	var tween = create_tween()
	tween.set_parallel()
	tween.tween_property(self, "position", goal_position, duration).set_trans(Tween.TRANS_QUAD)
	set_modulo_rotation(goal_rotation)
	var rot_tween = create_tween()
	rot_tween.tween_property(self, "rotation", goal_rotation, duration).set_trans(Tween.TRANS_QUAD)

# Return card1 < card2
func compare_cards(card1, card2, current_suit):
	if card1.suit == trump:
		return card2.suit == trump and Card.TRUMP_VALUES[card1.rank] < Card.TRUMP_VALUES[card2.rank]
	assert(card1.suit == current_suit)
	if card2.suit == trump:
		return true
	if card2.suit != current_suit:
		return false
	return Card.CARDS_VALUES[card1.rank] < Card.CARDS_VALUES[card2.rank]
