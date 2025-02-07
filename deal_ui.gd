extends Control
const CARD_SCENE = preload("res://card.tscn")

var current_trick = null
var hand_to_deal = 0

var all_hands = []

func _ready():
	all_hands = [$Bottom/Hand, $Left/Hand, $Top/Hand, $Right/Hand]
	get_tree().create_timer(0.1).timeout.connect(_on_start_pressed)

func deal_card() -> void:
	var dealt_card = $Deck/Deck.cards.pop_back()
	all_hands[hand_to_deal%4].add_card(dealt_card, true)#, hand_to_deal%4 == 0)
	hand_to_deal += 1


func _on_trick_trick_won(winner: int) -> void:
	all_hands[winner].add_trick($Deck/Trick)


func _on_start_pressed() -> void:
	$Deck/Deck.visible = true
	$Start.visible = false
	for i in range(20):
		get_tree().create_timer(Hand.CARD_MOVEMENT_DURATION / 4. * i).timeout.connect(deal_card)
	get_tree().create_timer(Hand.CARD_MOVEMENT_DURATION * 5).timeout.connect(start_trump_picker)

var trump_picker = null
func start_trump_picker():
	trump_picker = load("res://trump_picker.tscn").instantiate()
	$Deck.add_child(trump_picker)
	var dealt_card = $Deck/Deck.cards.pop_back()
	trump_picker.add_card(dealt_card)
	trump_picker.no_trump.connect(func(): get_tree().quit())
	trump_picker.player_chose.connect(trump_card_selected)

func trump_card_selected(player, card, trump):
	$Deck/TrumpBackground.set_trump(trump)
	all_hands[player].add_card(card, false)
	for hand in all_hands:
		hand.trump = trump
	trump_picker.queue_free()
	hand_to_deal = player + 1
	for i in range(11):
		get_tree().create_timer(Hand.CARD_MOVEMENT_DURATION / 4. * i).timeout.connect(deal_card)
	get_tree().create_timer(Hand.CARD_MOVEMENT_DURATION / 4. * 11 + Card.FLIP_DURATION).timeout.connect(start_round)

func start_round():
	var trick = load("res://trick.tscn").instantiate()
	trick.hands = all_hands
	$Deck.add_child(trick)
