extends Node2D

signal trick_finished(last_trick: int)

const RADIUS = 75
const THROW_RANDOMNESS = 20
const PLAY_RANDOM_TIMER = 0.25
const CARD_RETRIEVAL_WAIT = 0.25
const CARD_RETRIEVAL_TIME = 0.25
const FLIP_TIMER = 0.25
const CARD_TO_WINNER_WAIT = 0.25

var start_direction = 0
var current_direction = 0
var cards = []
var best_card_id = null
var trump = Card.Suit.SPADES
var hands = []
var ai_util = AiUtil.new()
var game_state: GdGameState

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	trump = hands[0].trump
	game_state = GdGameState.new()
	game_state.set_trump(trump)
	for hand in hands:
		hand.play_card.connect(add_card)
	for player in range(4):
		for card in hands[player].cards:
			game_state.add_card_to_player(player, card.suit, card.rank)
	start_trick()

func start_trick():
	best_card_id = null
	cards = []
	$Placeholder.visible = true
	$Placeholder.rotation = start_direction * PI/2
	$Placeholder.position = Vector2(cos($Placeholder.rotation + PI/2), sin($Placeholder.rotation + PI/2)) * RADIUS
	hands[start_direction].activate_all_cards()
	if start_direction != 0:
		get_tree().create_timer(PLAY_RANDOM_TIMER).timeout.connect(hands[start_direction].play_random)


func _process(_delta: float) -> void:
	$Placeholder.position = Vector2(cos($Placeholder.rotation + PI/2), sin($Placeholder.rotation + PI/2)) * RADIUS

const MOVE_SPEED = 0.25
func move_placeholder():
	var rot_tween = create_tween()
	rot_tween.tween_property($Placeholder, "rotation", (len(cards) + start_direction)*PI/2, MOVE_SPEED)


func add_card(card):
	print("card played")
	ChildExchange.exchange(card, self)
	cards.append(card)
	game_state.play_card(card.suit, card.rank)
	if best_card_id == null or compare_cards(cards[best_card_id], card, cards[0].suit):
		best_card_id = len(cards) - 1
	cards.back().tween_position($Placeholder.position, fmod($Placeholder.rotation, 2 * PI) + deg_to_rad(randi()%THROW_RANDOMNESS - THROW_RANDOMNESS / 2.), .5)
	cards.back().z_index = len(cards) - 1
	if len(cards) < 4:
		move_placeholder()
		var opponent_wins = len(cards) - 2 != best_card_id
		if len(hands[(len(cards) + start_direction)%4].cards) != 1:
			hands[(len(cards) + start_direction)%4].activate_possible_cards(cards[0].suit, opponent_wins, null if cards[best_card_id].suit != trump else Card.TRUMP_VALUES[cards[best_card_id].rank])
			if (len(cards) + start_direction)%4 != 0:
				hands[(len(cards) + start_direction)%4].play_ai(ai_util, cards)  # get_tree().create_timer(PLAY_RANDOM_TIMER).timeout.connect(hands[(len(cards) + start_direction)%4].play_random)
		else:
			hands[(len(cards) + start_direction)%4].activate_all_cards()
			get_tree().create_timer(PLAY_RANDOM_TIMER).timeout.connect(hands[(len(cards) + start_direction)%4].play_random)
	else:
		$Placeholder.visible = false
		cards[best_card_id].highlighted = true
		cards[best_card_id].z_index = 4
		get_tree().create_timer(CARD_RETRIEVAL_WAIT).timeout.connect(bring_all_cards)


func bring_all_cards():
	for card in cards:
		card.tween_position(Vector2.ZERO, 0, CARD_RETRIEVAL_TIME)
	get_tree().create_timer(CARD_RETRIEVAL_TIME + FLIP_TIMER).timeout.connect(flip)
func flip():
	for card in cards:
		card.flip()
	get_tree().create_timer(Card.FLIP_DURATION + CARD_TO_WINNER_WAIT).timeout.connect(give_all_card_to_winner)
func give_all_card_to_winner():
	for i in range(len(cards)):
		cards[i].z_index = len(cards)-i
		cards[i].highlighted = false
		get_tree().create_timer(Hand.CARD_MOVEMENT_DURATION * i).timeout.connect(
			func(): hands[(start_direction + best_card_id) % 4].add_card_won(cards[i]))
	get_tree().create_timer(Hand.CARD_MOVEMENT_DURATION * 4).timeout.connect(restart_trick)
func restart_trick():
	if len(hands[0].cards) == 0:
		trick_finished.emit((start_direction + best_card_id) % 2 == 0)
	else:
		start_direction = (start_direction + best_card_id) % 4
		start_trick()

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
