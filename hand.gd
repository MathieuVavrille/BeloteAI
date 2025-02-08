extends Node2D
class_name Hand

signal play_card(card: Card)

@export var small_radius = 100
@export var big_radius = 200
@export var natural_order = true

const CARD_SCENE = preload("res://card.tscn")
const CARD_MOVEMENT_DURATION = 0.1
var angle_between_cards = PI / 12

var cards = []
var cards_won = []
var points_won = 0
var trump = null:
	set(new_trump):
		trump = new_trump
		cards.sort_custom(Card.generate_order(trump))
		position_cards()

func send_play_card_signal(card):
	card.highlighted = false
	for j in range(len(cards)):
		cards[j].highlighted = false
	card.tween_scale(1)
	play_card.emit(card)
	position_cards()
	


func _input(event):
	if event is InputEventMouseButton and event.pressed:
		for i in range(len(cards)-1, -1, -1):
			if cards[i].is_hovered and cards[i].highlighted:
				var selected_card = cards.pop_at(i)
				send_play_card_signal(selected_card)
				return


func add_card(card, flip):
	ChildExchange.exchange(card, self)
	card.mouse_movement.connect(on_card_mouse_movement)
	cards.append(card)
	cards.sort_custom(Card.generate_order(trump))
	if flip:
		get_tree().create_timer(CARD_MOVEMENT_DURATION).timeout.connect(card.flip)
	position_cards()


func position_cards():
	for i in range(len(cards)):
		var angle = PI/2 - angle_between_cards * (i - (len(cards)-1) / 2.)
		var goal_position = Vector2(cos(angle) * big_radius, -sin(angle) * small_radius)
		var card_rotation = -small_radius**2 * goal_position.x / big_radius**2 / goal_position.y
		cards[i].tween_position(goal_position, atan(card_rotation), CARD_MOVEMENT_DURATION)
		get_tree().create_timer(CARD_MOVEMENT_DURATION / 2.).timeout.connect(func(): cards[i].z_index=i)


func swap_cards(i, j):
	var cardi = cards[i]
	cards[i] = cards[j]
	cards[j] = cardi

func on_card_mouse_movement():
	var can_hover = true
	for i in range(len(cards)-1, -1, -1):
		if cards[i].is_hovered and can_hover and cards[i].highlighted:
			cards[i].tween_scale(1.1)
			can_hover = false
		else:
			cards[i].tween_scale(1.)

func add_trick(trick):
	ChildExchange.exchange(trick, self)
	trick.tween_position(Vector2(big_radius, -small_radius / 2.), 0, 1.)

func add_card_won(card):
	points_won += max(0, Card.TRUMP_VALUES[card.rank] if card.suit == trump else Card.CARDS_VALUES[card.rank])
	ChildExchange.exchange(card, self)
	cards_won.append(card)
	card.tween_position(Vector2(big_radius, -small_radius / 2.), 0, CARD_MOVEMENT_DURATION)
	var current_len = len(cards)
	get_tree().create_timer(CARD_MOVEMENT_DURATION / 2.).timeout.connect(
		func(): card.z_index = current_len - 40)

func activate_possible_cards(selected_suit, opponent_wins, highest_trump):
	var has_better_trump = false
	var has_selected_suit = false
	# Find out if we have the selected suit
	for card in cards:
		if card.suit == trump and (highest_trump == null or Card.TRUMP_VALUES[card.rank] > highest_trump):
			has_better_trump = true
		if card.suit == selected_suit:
			has_selected_suit = true
	if selected_suit != trump:
		if has_selected_suit:
			for card in cards:
				if card.suit == selected_suit:
					card.highlighted = true
		else:  # I don't have the selected suit
			if opponent_wins and has_better_trump:
				for card in cards:
					if card.suit == trump and (highest_trump == null or Card.TRUMP_VALUES[card.rank] > highest_trump):
						card.highlighted = true
			else: # my teamate wins
				activate_all_cards()
	else:  # selected_suit == trump
		if has_better_trump:  # has a better one
			for card in cards:
				if card.suit == trump and (highest_trump == null or Card.TRUMP_VALUES[card.rank] > highest_trump):
					card.highlighted = true
		elif has_selected_suit:  # has suit but less
			for card in cards:
				if card.suit == trump:
					card.highlighted = true
		else:  # Not the suit
			activate_all_cards()


func activate_all_cards():
	for card in cards:
		card.highlighted = true

func play_random():
	var nb_highlighted_cards = 0
	for card in cards:
		if card.highlighted:
			nb_highlighted_cards += 1
	var card_to_play = randi() % nb_highlighted_cards
	for i in range(len(cards)):
		if cards[i].highlighted:
			if card_to_play == 0:
				send_play_card_signal(cards.pop_at(i))
				return
			else:
				card_to_play -= 1
